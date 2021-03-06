#ifndef COST_FUNCTION_PD_H
#define COST_FUNCTION_PD_H

#include <slam/icp/CostFunction.h>

namespace slam {

class CostFunctionPD : public CostFunction {
public:
  CostFunctionPD() : CostFunction() {}
  virtual ~CostFunctionPD() {}

  virtual void Initialize() override;
  virtual double CalcValue(double tx, double ty, double th) override;

  friend class CostFunctionPDTestFriend;
};

} // namespace slam
#endif /* COST_FUNCTION_PD_H */
