#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "algorithms/greedy/NearestNeighbour.h"
#include "OrienteeringProblemInputData.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m, py::mod_gil_not_used(), py::multiple_interpreters::per_interpreter_gil()) {
    m.doc() = R"pbdoc(
        Orienteering Solver Python Bindings
        -----------------------------------

        .. currentmodule:: orienteering_solver

        .. autosummary::
           :toctree: _generate

           nearest_neighbour
           OrienteeringProblemInputData
    )pbdoc";

    py::class_<OrienteeringProblemInputData>(m, "OrienteeringProblemInputData")
        .def(py::init<>())
        .def_readwrite("adjacency_matrix", &OrienteeringProblemInputData::adjacencyMatrix)
        .def_readwrite("profits", &OrienteeringProblemInputData::profits)
        .def_readwrite("budget", &OrienteeringProblemInputData::budget)
        .def_readwrite("start_node", &OrienteeringProblemInputData::startNode)
        .def_readwrite("end_node", &OrienteeringProblemInputData::endNode);

    m.def("nearest_neighbour", &nearestNeighbour, 
          "A function to find the nearest neighbour path given OP constraints.",
          py::arg("input_data"));

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
