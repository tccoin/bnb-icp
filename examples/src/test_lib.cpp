#include <bits/stdc++.h>
#include <ortools/linear_solver/linear_solver.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace operations_research {
  void LinearProgrammingExample() {
    std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
    if (!solver) {
      LOG(WARNING) << "SCIP solver unavailable.";
      return;
    }

    const double infinity = solver->infinity();
    // x and y are non-negative variables.
    MPVariable* const x1 = solver->MakeNumVar(0.0, infinity, "x1");
    MPVariable* const x2 = solver->MakeNumVar(0.0, infinity, "x2");
    MPVariable* const x3 = solver->MakeNumVar(0.0, infinity, "x3");
    MPVariable* const x4 = solver->MakeNumVar(0.0, infinity, "x4");
    LOG(INFO) << "Number of variables = " << solver->NumVariables();

    // 8*x1 + 5*x2 + 3*x3 + 2*x4 <= 10
    MPConstraint* const c0 = solver->MakeRowConstraint(-infinity, 10.0);
    c0->SetCoefficient(x1, 8);
    c0->SetCoefficient(x2, 5);
    c0->SetCoefficient(x3, 3);
    c0->SetCoefficient(x4, 2);
    LOG(INFO) << "Number of constraints = " << solver->NumConstraints();

    // Objective function: 15*x1 + 12*x2 + 4*x3 + 2*x4.
    MPObjective* const objective = solver->MutableObjective();
    objective->SetCoefficient(x1, 15);
    objective->SetCoefficient(x2, 12);
    objective->SetCoefficient(x3, 4);
    objective->SetCoefficient(x4, 2);
    objective->SetMaximization();

    const MPSolver::ResultStatus result_status = solver->Solve();
    // Check that the problem has an optimal solution.
    if (result_status != MPSolver::OPTIMAL) {
      LOG(FATAL) << "The problem does not have an optimal solution!";
    }

    LOG(INFO) << "Solution:";
    LOG(INFO) << "Optimal objective value = " << objective->Value();
    LOG(INFO) << x1->name() << " = " << x1->solution_value();
    LOG(INFO) << x2->name() << " = " << x2->solution_value();
    LOG(INFO) << x3->name() << " = " << x3->solution_value();
    LOG(INFO) << x4->name() << " = " << x4->solution_value();
  }
}  // namespace operations_research

int main(int argc, char** argv) {
  operations_research::LinearProgrammingExample();

  // import plt
  py::scoped_interpreter guard;
  // auto axes3D = pybind11::module::import("mpl_toolkits.mplot3d").attr("Axes3D");
  auto plt = py::module_::import("matplotlib.pyplot");
  auto fig = plt.attr("figure")("figsize"_a = py::make_tuple(10, 8));
  plt.attr("ion")();

  // plot
  std::vector<double> x_data = {1, 2, 3, 4, 5};
  std::vector<double> y_data = {5, 4, 3, 2, 1};
  auto ax = fig.attr("add_subplot")(1, 1, 1);
  ax.attr("plot")(x_data, y_data, "linestyle"_a = "-", "linewidth"_a = "1", "markersize"_a = "5",
                  "marker"_a = "o");
  plt.attr("show")();
  plt.attr("pause")(0);

  return 0;
}