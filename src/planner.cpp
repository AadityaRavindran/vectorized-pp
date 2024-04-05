#include "vectorized_pp/planner.hpp"

#include <boost/geometry/strategies/agnostic/buffer_distance_symmetric.hpp>
#include <boost/geometry/strategies/cartesian/buffer_end_flat.hpp>
#include <boost/geometry/strategies/cartesian/buffer_join_miter.hpp>
#include <boost/geometry/strategies/cartesian/buffer_side_straight.hpp>

namespace vectorized_pp
{
void Planner::updateConfig(
  const Polygons& polygons,
  const ZoneSpatialIndex& zones_spatial,
  const Config& config)
{
  config_ = config;
  polygons_ = polygons;
  zones_spatial_ = zones_spatial;

  for (auto & polygon : polygons_) {
    MultiPolygon inflated_polygon;
    bg::buffer(
      polygon,
      inflated_polygon,
      bg::strategy::buffer::distance_symmetric<double>(config_.inflation_distance),
      bg::strategy::buffer::side_straight(),
      bg::strategy::buffer::join_miter(),
      bg::strategy::buffer::end_flat(),
      bg::strategy::buffer::point_square());
    if (inflated_polygon.size() > 1)
      throw std::runtime_error("Inflated multipolygon is not a polygon");
    inflated_polygons_.push_back(inflated_polygon[0]);
  }
}

Linestring Planner::makePlan(const Point& start, const Point& goal) const
{
  Linestring plan;
  plan.push_back(start);
  plan.push_back(goal);
  bool intersections_exist = true;
  do {
    Zones intersections = getIntersections(plan);
    for (auto & [box, id] : intersections) {
      if (!resolveIntersections(plan, id, intersections.size()))
        throw std::runtime_error("Unable to resolve intersections");
    }
    intersections_exist = !intersections.empty();
  }while (intersections_exist);
  return plan;
}

Zones Planner::getIntersections(const Linestring& plan) const
{
  Zones intersections;
  zones_spatial_.query(bgi::intersects(plan), std::back_inserter(intersections));
  return intersections;
}

bool Planner::resolveIntersections(Linestring& plan, ID id, size_t num_intersections) const
{
  Linestring updated_plan = plan;
  Polygon inflated_polygon = inflated_polygons_[id];
  for (auto& inflated_point : inflated_polygon.outer()) {
    Point goal = updated_plan.back();
    updated_plan.pop_back();
    updated_plan.push_back(inflated_point);
    updated_plan.push_back(goal);
    if (getIntersections(updated_plan).size() < num_intersections) {
      plan = updated_plan;
      return true;
    }
  }
  return false;
}
}  // namespace vectorized_pp
