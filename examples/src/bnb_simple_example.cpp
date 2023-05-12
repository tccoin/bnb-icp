#include <bits/stdc++.h>
#include <ortools/linear_solver/linear_solver.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace operations_research;

class Node {
public:
  std::vector<double> region;
  std::vector<double> solution;
  bool isFeasible = true;
  int fixedVariable = 0;
  double objective = 0;
  Node(std::vector<double> region_) : region(region_) {
    for (double x : region) {
      if (x != -1) {
        fixedVariable++;
      }
    }
  }

  void updateSolution(bool isFeasible) { this->isFeasible = isFeasible; }

  void updateSolution(std::vector<double>& solution, double objective) {
    this->objective = objective;
    this->solution = solution;
    for (double x : solution) {
      if (x != 1 && x != 0) {
        isFeasible = false;
      }
    }
  }

  void branch(std::queue<Node>& q) {
    if (fixedVariable == 4) {
      return;
    }
    std::vector<double> subregion1 = region;
    std::vector<double> subregion2 = region;
    subregion1[fixedVariable] = 0;
    subregion2[fixedVariable] = 1;
    q.push(Node(subregion1));
    q.push(Node(subregion2));
  }
};

void solveLP(Node& node) {
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  if (!solver) {
    LOG(WARNING) << "SCIP solver unavailable.";
    node.updateSolution(false);
    return;
  }
  // Variables
  const double infinity = solver->infinity();
  std::vector<MPVariable*> variables;
  for (int i = 0; i < node.region.size(); i++) {
    double x = node.region[i];
    if (x == -1) {
      variables.push_back(solver->MakeNumVar(0, 1, "x" + std::to_string(i)));
    } else {
      variables.push_back(solver->MakeNumVar(x, x, "x" + std::to_string(i)));
    }
  }
  // LOG(INFO) << "Number of variables = " << solver->NumVariables();

  // Constraints
  // 8*x1 + 5*x2 + 3*x3 + 2*x4 <= 10
  MPConstraint* const c0 = solver->MakeRowConstraint(-infinity, 10.0);
  c0->SetCoefficient(variables[0], 8);
  c0->SetCoefficient(variables[1], 5);
  c0->SetCoefficient(variables[2], 3);
  c0->SetCoefficient(variables[3], 2);
  // LOG(INFO) << "Number of constraints = " << solver->NumConstraints();

  // Objective function
  // 15*x1 + 12*x2 + 4*x3 + 2*x4.
  MPObjective* const objective = solver->MutableObjective();
  objective->SetCoefficient(variables[0], 15);
  objective->SetCoefficient(variables[1], 12);
  objective->SetCoefficient(variables[2], 4);
  objective->SetCoefficient(variables[3], 2);
  objective->SetMaximization();

  // Solve
  const MPSolver::ResultStatus result_status = solver->Solve();
  // Check that the problem has an optimal solution.
  if (result_status != MPSolver::OPTIMAL) {
    // LOG(INFO) << "The problem does not have an optimal solution!";
    node.updateSolution(false);
    return;
  }

  // Solution
  LOG(INFO) << "Solution:";
  std::vector<double> solution;
  for (int i = 0; i < variables.size(); i++) {
    solution.push_back(variables[i]->solution_value());
    LOG(INFO) << variables[i]->name() << " = " << variables[i]->solution_value();
  }
  node.updateSolution(solution, objective->Value());
}

int main(int argc, char** argv) {
  std::queue<Node> q;
  Node optimal(std::vector<double>{-1, -1, -1, -1});
  q.push(optimal);

  while (!q.empty()) {
    Node node = q.front();
    solveLP(node);
    LOG(INFO) << "Current node objective: " << node.objective
              << ", Optimal objective: " << optimal.objective;
    if (node.objective >= optimal.objective) {
      if (node.isFeasible) {
        optimal = node;
        LOG(INFO) << "Updated optimal solution" << std::endl;
      } else {
        node.branch(q);
        LOG(INFO) << "Branching" << std::endl;
      }
    } else {
      LOG(INFO) << "Pruned" << std::endl;
    }
    q.pop();
  }

  LOG(INFO) << "Optimal solution: ";
  for (double x : optimal.solution) {
    LOG(INFO) << x << " ";
  }
  LOG(INFO) << "Optimal objective: " << optimal.objective;

  return 0;
}