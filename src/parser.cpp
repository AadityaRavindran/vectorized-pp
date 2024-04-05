#include "vectorized_pp/parser.hpp"

#include <string>

namespace vectorized_pp
{
json Parser::parseGeojson(std::string& geojson_string) {
  json geojson = json::parse(geojson_string);

  ID id = 0;
  // Parse each geometry in the GeometryCollection
  for (const auto& geometry_json : geojson.at("geometries")) {
    std::string type = geometry_json.at("type").get<std::string>();
    if (type == "Point") {
       Point point = parsePoint(geometry_json.at("coordinates"));
    } else if (type == "LineString") {
      Linestring linestring = parseLinestring(geometry_json.at("coordinates"));
    } else if (type == "Polygon") {
      Polygon polygon = parsePolygon(geometry_json.at("coordinates"));
      Box box = bg::return_envelope<Box>(polygon);
      polygons_.push_back(polygon);
      zones_spatial_.insert(std::make_pair(box, id++));
    } else {
      throw std::runtime_error("Unsupported geometry type " + type);
    }
  }
  return geojson;
}

Point Parser::parsePoint(const json& point_json) {
  return Point(point_json.at(0).get<float>(), point_json.at(1).get<float>());
}

Linestring Parser::parseLinestring(const json& linestring_json) {
  Linestring linestring;
  for (const auto& point_json : linestring_json) {
      linestring.push_back(parsePoint(point_json));
  }
  return linestring;
}

Polygon Parser::parsePolygon(const json& polygon_json) {
  Polygon polygon;
  for (const auto& ring_json : polygon_json) {
      Linestring ring = parseLinestring(ring_json);
      boost::geometry::append(polygon.outer(), ring);
  }
  return polygon;
}
}  // namespace vectorized_pp
