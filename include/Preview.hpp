#ifndef VRP_DRONE_PREVIEW_H
#define VRP_DRONE_PREVIEW_H

#include <iostream>
#include <fstream>
#include <utility>
#include "Graph/Vertex.hpp"
#include "Graph/Edge.hpp"

class Preview {
protected :
    std::ofstream m_file;

    const unsigned int m_xscale = 1;
    double m_latitude_init = 0;
    double m_longitude_init = 0;

    static double degree_to_meters(double x);
    double set_minimum(const std::vector<Vertex>& t_vertices);
    double getX(double x) const;
    double getY(double y) const;

    void new_line();
    void write_begin();
    virtual void write_tikz() = 0;
    void write_end();
    void draw_line(double t_x_first_point, double t_y_first_point,
                   double t_x_next_point, double t_y_next_point,
                   const std::string& t_task_id_for_color);
    void draw_node(double t_x_point, double t_y_point, const std::string& t_color);
    void write_vertices(const std::vector<Vertex>& t_vertices, const std::string& t_color_with="red", const std::string& t_color_without="blue");
    void write_edges(const std::vector<Edge>& t_edges, const std::vector<Vertex>& t_vertices, std::string t_color="black");

public:
    explicit Preview(const std::string& t_filename);

    void operator()();
};

#endif //VRP_DRONE_PREVIEW_H
