#ifndef PLANNER_HPP_
#define PLANNER_HPP_

#include "vectorized_pp/types.hpp"

namespace vectorized_pp
{

struct Config
{
  float inflation_distance { 0.5 };
};

class Planner
{
public:
  void updateConfig(const Polygons& polygons, const ZoneSpatialIndex& zones_spatial, const Config& config = Config());
  Linestring makePlan(const Point& start, const Point& goal) const;
  Zones getIntersections(const Linestring& plan) const;
  bool resolveIntersections(Linestring& plan, ID id, size_t num_intersections) const;
protected:
  Config config_;
  Polygons polygons_;
  Polygons inflated_polygons_;
  ZoneSpatialIndex zones_spatial_;
};
}  // namespace vectorized_pp
#endif  // PLANNER_HPP_
