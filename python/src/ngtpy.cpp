//
// Copyright (C) 2018-2019 Yahoo Japan Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include	"NGT/Index.h"

#include	<pybind11/pybind11.h>
#include	<pybind11/stl.h>
#include	<pybind11/numpy.h>

namespace py = pybind11;

class Index : public NGT::Index {
public:
  Index(
   const string path, 			// ngt index path.
   bool readOnly = false,		// read only or not.
   bool zeroBasedNumbering = true	// object ID numbering.
  ):NGT::Index(path, readOnly) {
    if (readOnly) {
      std::cerr << "ngtpy::Index read only!" << std::endl;
    }
    zeroNumbering = zeroBasedNumbering;
    numOfDistanceComputations = 0;
    numOfSearchObjects = 10;
    searchRadius = FLT_MAX;
  }

  static void create(
   const string path,
   size_t dimension,
   int edgeSizeForCreation = 10,
   int edgeSizeForSearch = 40,
   const string distanceType = "L2",
   const string objectType = "Float"
  ) {
    NGT::Property prop;
    prop.dimension = dimension;
    prop.edgeSizeForCreation = edgeSizeForCreation;
    prop.edgeSizeForSearch = edgeSizeForSearch;

    if (objectType == "Float" || objectType == "float") {
      prop.objectType = NGT::Index::Property::ObjectType::Float;
    } else if (objectType == "Byte" || objectType == "byte") {
      prop.objectType = NGT::Index::Property::ObjectType::Uint8;
    } else {
      std::cerr << "ngtpy::create: invalid object type. " << objectType << std::endl;
      return;
    }

    if (distanceType == "L1") {
      prop.distanceType = NGT::Index::Property::DistanceType::DistanceTypeL1;
    } else if (distanceType == "L2") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeL2;
    } else if (distanceType == "Hamming") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeHamming;
    } else if (distanceType == "Jaccard") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeJaccard;
    } else if (distanceType == "Angle") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeAngle;
    } else if (distanceType == "Normalized Angle") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeNormalizedAngle;
    } else if (distanceType == "Cosine") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeCosine;
    } else if (distanceType == "Normalized Cosine") {
      prop.distanceType = NGT::Property::DistanceType::DistanceTypeNormalizedCosine;
    } else {
      std::cerr << "ngtpy::create: invalid distance type. " << distanceType << std::endl;
      return;
    }
    NGT::Index::createGraphAndTree(path, prop);
  }

  void batchInsert(
   py::array_t<double> objects, 
   size_t numThreads = 8,
   bool debug = false
  ) {
    py::buffer_info info = objects.request();
    if (debug) {
      std::cerr << info.shape.size() << ":" << info.shape[0] << ":" << info.shape[1] << std::endl;
    }
    auto ptr = static_cast<double *>(info.ptr);
    assert(info.shape.size() == 2);
    NGT::Property prop;
    getProperty(prop);
    if (prop.dimension != info.shape[1]) {
      std::cerr << "ngtpy::insert: Error! dimensions are inconsitency. " << prop.dimension << ":" << info.shape[1] << std::endl;
      return;
    }
    NGT::Index::append(ptr, info.shape[0]);
    NGT::Index::createIndex(numThreads);
    numOfDistanceComputations = 0;
  }

  int insert(
   py::array_t<double> object, 
   bool debug = false
  ) {
    py::buffer_info info = object.request();
    auto ptr = static_cast<double *>(info.ptr);
    if (debug) {
      std::cerr << info.shape.size() << ":" << info.shape[0] << ":" << info.shape[1] << std::endl;
      for (int i = 0; i < info.shape[0]; i++) {
	std::cerr << *(ptr + i) << " ";
      }
      std::cerr << std::endl;
    }
    std::vector<double> v(ptr, ptr + info.shape[0]);
    return NGT::Index::insert(v);
    numOfDistanceComputations = 0;
  }

  py::object search(
   py::object query,
   size_t size = 10, 			// the number of resultant objects
   float epsilon = 0.1, 		// search parameter epsilon. the adequate range is from 0.0 to 0.15. minus value is acceptable.
   int edgeSize = -1,			// the number of used edges for each node during the exploration of the graph.
   bool withDistance = true
  ) {
    py::array_t<float> qobject(query);
    py::buffer_info qinfo = qobject.request();
    NGT::Object *ngtquery = 0;
    try {
      ngtquery = NGT::Index::allocateObject(static_cast<float*>(qinfo.ptr), qinfo.size);
    } catch (NGT::Exception &e) {
      std::cerr << e.what() << endl;
      if (!withDistance) {
	return py::array_t<int>();
      } else {
	return py::list();
      }
    }
    NGT::SearchContainer sc(*ngtquery);
    if (size == 0) {
      sc.setSize(numOfSearchObjects);		// the number of resulting objects.
    } else {
      sc.setSize(size);				// the number of resulting objects.
    }
    sc.setRadius(searchRadius);
    sc.setEpsilon(epsilon);			// set exploration coefficient.
    sc.setEdgeSize(edgeSize);			// if maxEdge is minus, the specified value in advance is used.
    NGT::ObjectDistances objects;
    sc.setResults(&objects);

//    NGT::Index::linearSearch(sc);
    NGT::Index::search(sc);

    numOfDistanceComputations += sc.distanceComputationCount;

    NGT::Index::deleteObject(ngtquery);
    if (!withDistance) {
//      NGT::ResultPriorityQueue &r = sc.getWorkingResult();
//      py::array_t<int> ids(r.size());
      py::array_t<int> ids(objects.size());
      py::buffer_info idsinfo = ids.request();
//      int *endptr = reinterpret_cast<int*>(idsinfo.ptr);
//      int *ptr = endptr + (r.size() - 1);
//      if (zeroNumbering) {
//        while (ptr >= endptr) {
//	  *ptr-- = r.top().id - 1;
//	  r.pop();
//        }
//      } else {
//        while (ptr >= endptr) {
//	  *ptr-- = r.top().id;
//	  r.pop();
//        }
      int *ptr = reinterpret_cast<int*>(idsinfo.ptr);
      for (size_t oidx = 0; oidx < objects.size(); ++oidx) {
        ptr[oidx] = objects[oidx].id - 1;
      }

      return ids;
    }
    py::list results;
    NGT::ObjectDistances r;
    r.moveFrom(sc.getWorkingResult());
    if (zeroNumbering) {
      for (auto ri = r.begin(); ri != r.end(); ++ri) {
	results.append(py::make_tuple((*ri).id - 1, (*ri).distance));
      }
    } else {
      for (auto ri = r.begin(); ri != r.end(); ++ri) {
	results.append(py::make_tuple((*ri).id, (*ri).distance));
      }
    }
    return results;
  }

    py::object searchRange(
   py::object query,
   float radius = 0.4, 			// the number of resultant objects
   float epsilon = 0.1, 		// search parameter epsilon. the adequate range is from 0.0 to 0.15. minus value is acceptable.
   int edgeSize = -1,			// the number of used edges for each node during the exploration of the graph.
   bool withDistance = true,
   float delta = 1e-10
  ) {
    py::array_t<float> qobject(query);
    py::buffer_info qinfo = qobject.request();
    NGT::Object *ngtquery = 0;
    int size = 100;
    while(1) {
        try {
          ngtquery = NGT::Index::allocateObject(static_cast<float*>(qinfo.ptr), qinfo.size);
        } catch (NGT::Exception &e) {
          std::cerr << e.what() << endl;
          if (!withDistance) {
        return py::array_t<int>();
          } else {
        return py::list();
          }
        }
        NGT::SearchContainer sc(*ngtquery);
        if (size == 0) {
          sc.setSize(numOfSearchObjects);		// the number of resulting objects.
        } else {
          sc.setSize(size);				// the number of resulting objects.
        }
        sc.setRadius(searchRadius);
        sc.setEpsilon(epsilon);			// set exploration coefficient.
        sc.setEdgeSize(edgeSize);			// if maxEdge is minus, the specified value in advance is used.
        NGT::ObjectDistances objects;
        sc.setResults(&objects);

    //    NGT::Index::linearSearch(sc);
        NGT::Index::search(sc);

        numOfDistanceComputations += sc.distanceComputationCount;

        NGT::Index::deleteObject(ngtquery);
        if (!withDistance) {
    //      NGT::ResultPriorityQueue &r = sc.getWorkingResult();
    //      py::array_t<int> ids(r.size());
          size_t res_size = 0;
          for (size_t oidx = 0; oidx < objects.size() && objects[oidx].distance < radius + delta; ++oidx) res_size++;
          if (res_size == size){
            size *= 2;
            cout << "searchRange size doubled: " << size << endl;
            continue;
          }
          py::array_t<int> ids(res_size);
          py::buffer_info idsinfo = ids.request();
    //      int *endptr = reinterpret_cast<int*>(idsinfo.ptr);
    //      int *ptr = endptr + (r.size() - 1);
    //      if (zeroNumbering) {
    //        while (ptr >= endptr) {
    //	  *ptr-- = r.top().id - 1;
    //	  r.pop();
    //        }
    //      } else {
    //        while (ptr >= endptr) {
    //	  *ptr-- = r.top().id;
    //	  r.pop();
    //        }
          int *ptr = reinterpret_cast<int*>(idsinfo.ptr);
          for (size_t res_it = 0, oidx = 0; oidx < objects.size() && objects[oidx].distance < radius + delta; ++res_it, ++oidx) {
            ptr[res_it] = objects[oidx].id - 1;
          }

          return ids;
        }

        py::list results;
        NGT::ObjectDistances r;
        r.moveFrom(sc.getWorkingResult());
        if (zeroNumbering) {
          for (auto ri = r.begin(); ri != r.end() && (*ri).distance < radius + delta; ++ri)
             results.append(py::make_tuple((*ri).id - 1, (*ri).distance));
        } else {
          for (auto ri = r.begin(); ri != r.end() && (*ri).distance < radius + delta; ++ri)
              results.append(py::make_tuple((*ri).id, (*ri).distance));
        }
        if (results.size() == size)
          size *= 2;
        else
          return results;
    }
  }


  void remove(size_t id) {
    id = zeroNumbering ? id + 1 : id;
    NGT::Index::remove(id);
  }

  vector<float> getObject(size_t id) {
    id = zeroNumbering ? id + 1 : id;
    NGT::Property prop;
    NGT::Index::getProperty(prop);
    vector<float> object;
    object.reserve(prop.dimension);
    switch (prop.objectType) {
    case NGT::ObjectSpace::ObjectType::Uint8:
      {
	auto *obj = static_cast<uint8_t*>(NGT::Index::getObjectSpace().getObject(id));
	for (int i = 0; i < prop.dimension; i++) {
	  object.push_back(*obj++);
	}
	break;
      }
    default:
    case NGT::ObjectSpace::ObjectType::Float:
      {
	auto *obj = static_cast<float*>(NGT::Index::getObjectSpace().getObject(id));
	for (int i = 0; i < prop.dimension; i++) {
	  object.push_back(*obj++);
	}
	break;
      }
    }
    return object;
  }

  void set(size_t k, NGT::Distance r) {
    if (k > 0) {
      numOfSearchObjects = k;
    }
    if (r >= 0.0) {
      searchRadius = r;
    }
  }

  size_t	getNumOfDistanceComputations() { return numOfDistanceComputations; }

  bool		zeroNumbering;	// for object ID numbering. zero-based or one-based numbering.
  size_t	numOfDistanceComputations;
  size_t	numOfSearchObjects; // k
  NGT::Distance	searchRadius; // range query
};


PYBIND11_MODULE(ngtpy, m) {
    m.doc() = "ngt python";

    m.def("create", &::Index::create, 
          py::arg("path"), 
          py::arg("dimension"), 
          py::arg("edge_size_for_creation") = 10, 
          py::arg("edge_size_for_search") = 40, 
          py::arg("distance_type") = "L2", 
          py::arg("object_type") = "Float");

    py::class_<Index>(m, "Index")
      .def(py::init<const std::string &, bool, bool>(), 
           py::arg("path"),
           py::arg("read_only") = false,
           py::arg("zero_based_numbering") = true)
      .def("search", &::Index::search, 
           py::arg("query"), 
           py::arg("size") = 10, 
           py::arg("epsilon") = 0.1, 
           py::arg("edge_size") = -1,
           py::arg("with_distance") = true)
      .def("searchRange", &::Index::searchRange,
           py::arg("query"),
           py::arg("radius") = 0.4,
           py::arg("epsilon") = 0.1,
           py::arg("edge_size") = -1,
           py::arg("with_distance") = true,
           py::arg("delta") = 1e-10)
      .def("get_num_of_distance_computations", &::Index::getNumOfDistanceComputations)
      .def("save", &NGT::Index::save)
      .def("close", &NGT::Index::close)
      .def("remove", &::Index::remove, 
           py::arg("object_id"))
      .def("build_index", &NGT::Index::createIndex, 
           py::arg("num_threads") = 8)
      .def("get_object", &::Index::getObject, 
           py::arg("object_id"))
      .def("batch_insert", &::Index::batchInsert, 
           py::arg("objects"),
           py::arg("num_threads") = 8, 
           py::arg("debug") = false)
      .def("insert", &::Index::insert, 
           py::arg("object"),
           py::arg("debug") = false)
      .def("set", &::Index::set,
           py::arg("num_of_search_objects") = 0,
	   py::arg("search_radius") = -1.0);
}



