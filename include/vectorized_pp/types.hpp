#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <vector>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <nlohmann/json.hpp>

namespace vectorized_pp
{
using ID = unsigned;
using json = nlohmann::json;
namespace bg = boost::geometry;
namespace bgi = bg::index;
namespace bgm = bg::model;
using Point = bgm::d2::point_xy<double>;
using Linestring = bgm::linestring<Point>;
using Polygon = bgm::polygon<Point, false, true>;
using MultiPolygon = bgm::multi_polygon<Polygon>;
using Polygons = std::vector<Polygon>;
using Box = bgm::box<Point>;
using Zone = std::pair<Box, ID>;
using Zones = std::vector<Zone>;
using ZoneSpatialIndex = bgi::rtree<Zone, bgi::quadratic<16>>;
}  // namespace vectorized_pp
#endif  // TYPES_HPP_
