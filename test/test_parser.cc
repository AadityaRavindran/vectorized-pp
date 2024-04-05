#include <gtest/gtest.h>

#include <vectorized_pp/parser.hpp>

namespace vectorized_pp
{
class GeoJsonParsingTest : public ::testing::Test {
protected:
  void SetUp() override {
  std::string geoJsonString = R"(
    {
      "type": "GeometryCollection",
      "geometries": [
        {
          "type": "Point",
           "coordinates": [100.0, 0.0]
        },
        {
          "type": "LineString",
          "coordinates": [[101.0, 0.0], [102.0, 1.0]]
        },
        {
          "type": "Polygon",
          "coordinates": [[[100.0, 0.0], [101.0, 0.0], [101.0, 1.0], [100.0, 1.0], [100.0, 0.0]]]
        }
      ]
    }
   )";
  geoJson_ = parser_.parseGeojson(geoJsonString);
  }

  Parser parser_;
  json geoJson_;

};

// Test parsing of Point geometries
TEST_F(GeoJsonParsingTest, ParsePoint) {
  Point point = parser_.parsePoint(geoJson_.at("geometries")[0].at("coordinates"));
  ASSERT_DOUBLE_EQ(point.x(), 100.0);
  ASSERT_DOUBLE_EQ(point.y(), 0.0);
}

// Test parsing of LineString geometries
TEST_F(GeoJsonParsingTest, ParseLinestring) {
  Linestring linestring = parser_.parseLinestring(geoJson_.at("geometries")[1].at("coordinates"));
  ASSERT_EQ(linestring.size(), 2);
  ASSERT_DOUBLE_EQ(linestring[0].x(), 101.0);
  ASSERT_DOUBLE_EQ(linestring[0].y(), 0.0);
  ASSERT_DOUBLE_EQ(linestring[1].x(), 102.0);
  ASSERT_DOUBLE_EQ(linestring[1].y(), 1.0);
}

// Test parsing of Polygon geometries
TEST_F(GeoJsonParsingTest, ParsePolygon) {
  Polygon polygon = parser_.parsePolygon(geoJson_.at("geometries")[2].at("coordinates"));
  ASSERT_EQ(bg::num_points(polygon.outer()), 5);
  ASSERT_DOUBLE_EQ(bg::get<0>(polygon.outer()[0]), 100.0);
  ASSERT_DOUBLE_EQ(bg::get<1>(polygon.outer()[0]), 0.0);
  ASSERT_DOUBLE_EQ(bg::get<0>(polygon.outer()[1]), 101.0);
  ASSERT_DOUBLE_EQ(bg::get<1>(polygon.outer()[1]), 0.0);
  ASSERT_DOUBLE_EQ(bg::get<0>(polygon.outer()[2]), 101.0);
  ASSERT_DOUBLE_EQ(bg::get<1>(polygon.outer()[2]), 1.0);
  ASSERT_DOUBLE_EQ(bg::get<0>(polygon.outer()[3]), 100.0);
  ASSERT_DOUBLE_EQ(bg::get<1>(polygon.outer()[3]), 1.0);
  ASSERT_DOUBLE_EQ(bg::get<0>(polygon.outer()[4]), 100.0);
  ASSERT_DOUBLE_EQ(bg::get<1>(polygon.outer()[4]), 0.0);
}
}  // namespace vectorized_pp
