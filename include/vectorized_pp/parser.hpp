#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>

#include "vectorized_pp/types.hpp"

namespace vectorized_pp
{
class Parser
{
public:
json parseGeojson(std::string& geojson);
// Parse a GeoJSON Point into a Boost.Geometry point
Point parsePoint(const json& point_json);
// Parse a GeoJSON Linestring into a Boost.Geometry point
Linestring parseLinestring(const json& linestring_json);
// Parse a GeoJSON Polygon into a Boost.Geometry point
Polygon parsePolygon(const json& polygon_json);

Polygons getPolygons() const { return polygons_; }
ZoneSpatialIndex getZoneSpatialIndex() const { return zones_spatial_; }

protected:
  Polygons polygons_;
  ZoneSpatialIndex zones_spatial_;
};
}  // namespace vectorized_pp
#endif  // PARSER_HPP_
