#include <bits/stdc++.h>
#include <ceres/ceres.h>
#include <glog/logging.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;
namespace py = pybind11;
using namespace pybind11::literals;

// A templated cost functor that implements the residual r = 10 -
// x. The method operator() is templated so that we can then use an
// automatic differentiation wrapper around it to generate its
// derivatives.

struct CostFunctor {
  template <typename T> bool operator()(const T* const x, T* residual) const {
    residual[0] = 10.0 - x[0];
    return true;
  }
};

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  // The variable to solve for with its initial value. It will be
  // mutated in place by the solver.
  double x = 0.5;
  const double initial_x = x;
  // Build the problem.
  Problem problem;
  // Set up the only cost function (also known as residual). This uses
  // auto-differentiation to obtain the derivative (jacobian).
  CostFunction* cost_function = new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
  problem.AddResidualBlock(cost_function, nullptr, &x);
  // Run the solver!
  Solver::Options options;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.BriefReport() << "\n";
  std::cout << "x : " << initial_x << " -> " << x << "\n";

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