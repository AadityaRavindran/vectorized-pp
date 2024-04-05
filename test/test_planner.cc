#include <gtest/gtest.h>

#include <iostream>

#include <vectorized_pp/parser.hpp>
#include <vectorized_pp/planner.hpp>

namespace vectorized_pp
{

struct Params
{
  Point start;
  Point goal;
  size_t plan_size;
};

class PlannerTest : public ::testing::TestWithParam<Params> {
protected:
  void SetUp() override {
  std::string geoJsonString = R"(
    {
      "type": "GeometryCollection",
      "geometries": [
        {
          "type": "Polygon",
          "coordinates": [[[100.0, 0.0], [101.0, 0.0], [101.0, 1.0], [100.0, 1.0], [100.0, 0.0]]]
        },
        {
          "type": "Polygon",
          "coordinates": [[[100.0, 4.0], [101.0, 4.0], [101.0, 5.0], [100.0, 5.0], [100.0, 4.0]]]
        }
      ]
    }
   )";
   parser_.parseGeojson(geoJsonString);
   planner_.updateConfig(parser_.getPolygons(), parser_.getZoneSpatialIndex());
  }

  Parser parser_;
  Planner planner_;
};

TEST_P(PlannerTest, CollisionsCheck) {
  const auto &[start, goal, plan_size] = GetParam();
  Linestring plan = planner_.makePlan(start, goal);
  EXPECT_EQ(plan.size(), plan_size);
  Zones intersections = planner_.getIntersections(plan);
  if (!plan.empty()) {
    EXPECT_EQ(intersections.size(), 0U);
    for (auto intersection : intersections)
      std::cout << bg::dsv(intersection.first) << std::endl;
  }
}

INSTANTIATE_TEST_CASE_P(
  CollisionsCheck,
  PlannerTest,
  ::testing::Values(
    Params{{99.0, -1.0}, {102.0, -1.0}, 2},
    Params{{99.0, -1.0}, {102.0, 3.0}, 6},
    Params{{99.0, -1.0}, {102.0, 9.0}, 6}
  ));
}  // namespace vectorized_pp
