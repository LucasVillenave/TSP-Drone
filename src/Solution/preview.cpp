#include "Solution/preview.hpp"

#include <utility>

class Preview {

    std::ofstream m_file;
    const Solution m_solution;

    const unsigned int m_xscale = 1;
    const double m_latitude_init = 44.83;
    const double m_longitude_init = 0.53;

    double get_latitude(double x);
    double get_longitude(double x);

    void new_line();
    void write_begin();
    void write_tikz();
    void write_end();
    void write_frame();
    void draw_line(double t_x_first_point, double t_y_first_point,
                   double t_x_next_point, double t_y_next_point,
                   std::string t_task_id_for_color);
    void draw_node(double t_x_point, double t_y_point);

public:
    explicit Preview(Solution t_solution);

    void operator()();
};

double Preview::get_latitude(double x)
{
    return 1000*(x - m_latitude_init);
}

double Preview::get_longitude(double x)
{
    return 1000*(x - m_longitude_init);
}

Preview::Preview(Solution t_solution)
        : m_solution(std::move(t_solution))
{
    m_file.open("solution.tex");
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

void preview(Solution t_solution)
{
    Preview(std::move(t_solution))();
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

void Preview::write_tikz()
{
    m_file << "\\begin{tikzpicture}[xscale=" << m_xscale << "]\n";
    for(auto& v : m_solution.getVertices())
    {
        double x = get_longitude(v.getLongitude()),
            y = get_latitude(v.getLatitude());
        draw_node(x, y);
    }
    for(auto& e : m_solution.getEdges())
    {
        auto i = m_solution.getVertices()[e.getStartID()];
        auto l_i = get_longitude(i.getLongitude());
        auto L_i = get_latitude(i.getLatitude());
        auto j = m_solution.getVertices()[e.getEndID()];
        auto l_j = get_longitude(j.getLongitude());
        auto L_j = get_latitude(j.getLatitude());
        draw_line(l_i, L_i, l_j, L_j, "black");
    }
    new_line();
    write_frame();
}

void Preview::write_frame()
{
    //draw truck road
}

void Preview::write_end()
{
    m_file << "\\end{tikzpicture}\n"
           << "\n"
           << "\\end{document}";
}

void Preview::draw_line(double t_x_first_point, double t_y_first_point,
                        double t_x_next_point, double t_y_next_point,
                        std::string t_color)
{
    m_file << "  \\draw[" << t_color << ", thick] (" << t_x_first_point << "," << t_y_first_point
           << ") -- (" << t_x_next_point << "," << t_y_next_point << ");\n";
}

void Preview::draw_node(double t_x_point, double t_y_point)
{
    m_file << "  \\filldraw[blue] (" << t_x_point << ", " << t_y_point << ") circle (2pt);\n";
}