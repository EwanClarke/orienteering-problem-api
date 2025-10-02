#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "algorithms/greedy/NearestNeighbour.h"
#include "algorithms/greedy/InsertionHeuristic.h"
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

    m.def("insertion_heuristic", &insertionHeuristic, 
          "A function to find a path using a selected insertion heuristic given OP constraints.",
          py::arg("input_data"), py::arg("insertion_heuristic_function"));
    
    m.def("find_cheapest_insertion", &findCheapestInsertion, 
          "A function to find the cheapest insertion given a path and OP constraints.",
          py::arg("problem_data"), py::arg("path"), py::arg("visited"), py::arg("remaining_budget"));

    m.def("find_most_profitable_insertion", &findMostProfitableInsertion, 
          "A function to find the most profitable insertion given a path and OP constraints.",
            py::arg("problem_data"), py::arg("path"), py::arg("visited"), py::arg("remaining_budget"));

    m.def("find_greedy_insertion", &findGreedyInsertion, 
          "A function to find a greedy insertion given a path and OP constraints.",
            py::arg("problem_data"), py::arg("path"), py::arg("visited"), py::arg("remaining_budget"));

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
