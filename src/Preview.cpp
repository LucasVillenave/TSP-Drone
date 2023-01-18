#include <cmath>
#include "Preview.hpp"

double Preview::degree_to_meters(double x)
{
    // 1' = 1852m
    return 60*1852*x;
}

double Preview::set_minimum(const std::vector<Vertex> &t_vertices)
{
    double min_lat = 90.0, min_lon = 180.0;
    for(auto& v : t_vertices)
    {
        if(v.getLatitude() < min_lat)
            min_lat = v.getLatitude();
        if(v.getLongitude() < min_lon)
            min_lon = v.getLongitude();
    }
    min_lat = floor(min_lat*100)/100;
    min_lon = floor(min_lon*100)/100;
    m_latitude_init = min_lat;
    m_longitude_init = min_lon;

}

double Preview::getY(double y) const
{
    return degree_to_meters(y - m_latitude_init) / 100;
}

double Preview::getX(double x) const
{
    return degree_to_meters(x - m_longitude_init)/100;
}

void Preview::new_line()
{
    m_file << "\n";
}

void Preview::write_begin()
{
    m_file << "\\documentclass[border=2pt]{standalone}\n"
           << "\n"
           << "\\usepackage{tikz}\n"
           << "\\usepackage{xcolor}\n";
    new_line();
    m_file << "\\begin{document}\n";
    new_line();
}

void Preview::write_end()
{
    m_file << "\\end{tikzpicture}\n"
           << "\n"
           << "\\end{document}";
}

void Preview::draw_line(double t_x_first_point, double t_y_first_point,
                                double t_x_next_point, double t_y_next_point,
                                const std::string& t_color)
{
    m_file << "  \\draw[" << t_color << ", thick] (" << t_x_first_point << "," << t_y_first_point
           << ") -- (" << t_x_next_point << "," << t_y_next_point << ");\n";
}

void Preview::draw_node(double t_x_point, double t_y_point, const std::string& t_color)
{
    m_file << "  \\filldraw[" << t_color << "] (" << t_x_point << ", " << t_y_point << ") circle (3pt);\n";
}

void Preview::write_vertices(const std::vector<Vertex> &t_vertices, const std::string& t_color_with, const std::string& t_color_without)
{
    for(auto& v : t_vertices)
    {
        double x = getX(v.getLongitude()),
                y = getY(v.getLatitude());
        draw_node(x, y, t_color_without);

        if(v.getTDA() > 0)
        {
            draw_node(x, y, t_color_with);
        }
        else
        {
            draw_node(x, y, t_color_without);
        }
    }
}

void Preview::write_edges(const std::vector<Edge> &t_edges, const std::vector<Vertex>& t_vertices, std::string t_color)
{
    for(auto& e : t_edges)
    {
        auto i = t_vertices[e.getStartID()];
        auto l_i = getX(i.getLongitude());
        auto L_i = getY(i.getLatitude());
        auto j = t_vertices[e.getEndID()];
        auto l_j = getX(j.getLongitude());
        auto L_j = getY(j.getLatitude());
        draw_line(l_i, L_i, l_j, L_j, t_color);
    }
}

Preview::Preview(const std::string& t_filename)
{
    m_file.open(t_filename + ".tex");
    if (!m_file.is_open())
    {
        throw std::runtime_error("Couldn't open file");
    }
}

void Preview::operator()()
{
    write_begin();
    new_line();
    write_tikz();
    new_line();
    write_end();
    m_file.close();
}