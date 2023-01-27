#include "Solution/SolutionPreview.hpp"

#include <utility>

class SolutionPreview : public Preview {

    const std::string COLOR_TRUCK = "magenta";
    const std::string COLOR_DRONE1 = "green";
    const std::string COLOR_DRONE2 = "yellow";

    const Solution m_solution;
    std::vector<int> m_demands;
    std::vector<Position> m_nodes;
    std::vector<Event> m_truck_moves;
    std::vector<Event> m_truck_deliveries;
    std::vector<std::vector<Position>> m_drone1_moves;
    std::vector<std::vector<Position>> m_drone2_moves;
    std::vector<Event> m_drone_deliveries;

    void write_tikz() override;
    void write_moves();
    void write_truck_moves(const Event& t_event);
    void write_drone_moves(const std::vector<Position>& t_move, const std::string& t_color, const std::string& t_start_place, const std::string& t_end_place);
    void write_deliveries();
    void write_nodes();
    void write_legend();
    void draw_node_with_label(double t_x_point, double t_y_point, const std::string& t_color, const std::string& t_place, const std::string& t_name);
    void draw_arc(double t_x_first_point, double t_y_first_point, double t_x_next_point, double t_y_next_point, const std::string& t_color);

public:
    explicit SolutionPreview(Solution t_solution, const std::string& t_filename);

};


SolutionPreview::SolutionPreview(Solution t_solution, const std::string& t_filename)
        : Preview(t_filename), m_solution(std::move(t_solution))
{
    set_minimum(m_solution.getVertices());
    for(const auto& demand : m_solution.getDemands())
    {
        m_demands.emplace_back(demand.getAmount());
    }
    std::vector<Position> drone1_current_move;
    std::vector<Position> drone2_current_move;
    for(auto& event : m_solution.getEvents())
    {
        switch (event.getEventType()) {
            case 0:
                m_truck_moves.emplace_back(event);
                break;
            case 1:
                m_nodes.emplace_back(event.getPos1());
                break;
            case 2:
                if(event.getDroneID() == 0)
                {
                    drone1_current_move.emplace_back(event.getPos1());
                    drone1_current_move.emplace_back(m_solution.getDemands()[event.getDemandID()].getPos());
                }
                else
                {
                    drone2_current_move.emplace_back(event.getPos1());
                    drone2_current_move.emplace_back(m_solution.getDemands()[event.getDemandID()].getPos());
                }
                break;
            case 3:
                if(event.getDroneID() == 0)
                {
                    drone1_current_move.emplace_back(event.getPos1());
                    m_drone1_moves.emplace_back(drone1_current_move);
                    drone1_current_move = {};
                }
                else
                {
                    drone2_current_move.emplace_back(event.getPos1());
                    m_drone2_moves.emplace_back(drone2_current_move);
                    drone2_current_move = {};
                }
                break;
            case 4:
                m_truck_deliveries.emplace_back(event);
                break;
            case 5:
                m_drone_deliveries.emplace_back(event);
                break;
            default:
                continue;
        }
    }
}

void solutionPreview(Solution t_solution, const std::string& t_filename)
{
    SolutionPreview(std::move(t_solution), t_filename)();
}

void SolutionPreview::write_tikz()
{
    m_file << "\\usetikzlibrary{decorations.markings}\n";
    m_file << "\\begin{tikzpicture}[xscale=1]\n";
    write_moves();
    new_line();
    write_nodes();
    write_deliveries();
    write_vertices({m_solution.getVertices()[0]});
    new_line();
    write_legend();
}

void SolutionPreview::write_moves()
{
    m_file << "\\begin{scope}[very thick,decoration={markings,\n"
           << "    mark=at position 0.55 with {\\arrow{>}}} ] \n";
    for(const auto& event : m_truck_moves)
        write_truck_moves(event);
    for(const auto& move : m_drone1_moves)
        write_drone_moves(move, COLOR_DRONE1, "above", "right");
    for(const auto& move : m_drone2_moves)
        write_drone_moves(move, COLOR_DRONE2, "below", "left");
    m_file << "\\end{scope}\n";
}

void SolutionPreview::write_truck_moves(const Event& t_event)
{
    const double l_i = getX(t_event.getPos1().getLongitude());
    const double L_i = getY(t_event.getPos1().getLatitude());
    const double l_j = getX(t_event.getPos2().getLongitude());
    const double L_j = getY(t_event.getPos2().getLatitude());
    draw_arc(l_i, L_i, l_j, L_j, COLOR_TRUCK);
}

void SolutionPreview::write_drone_moves(const std::vector<Position>& t_move, const std::string& t_color, const std::string& t_start_place, const std::string& t_end_place)
{
    const auto& start = t_move[0];
    const double x_start = getX(start.getLongitude());
    const double y_start = getY(start.getLatitude());

    const auto& middle = t_move[1];
    const double x_middle = getX(middle.getLongitude());
    const double y_middle = getY(middle.getLatitude());

    const auto& finish = t_move[2];
    const double x_finish = getX(finish.getLongitude());
    const double y_finish = getY(finish.getLatitude());

    draw_node_with_label(x_start, y_start, t_color, t_start_place, "-");
    draw_arc(x_start, y_start, x_middle, y_middle, t_color);
    draw_arc(x_middle, y_middle, x_finish, y_finish, t_color);
    draw_node_with_label(x_finish, y_finish, t_color, t_end_place, "+");
}

void SolutionPreview::write_deliveries()
{
    for(auto& event : m_truck_deliveries)
    {
        double l_i = getX(event.getPos1().getLongitude());
        double L_i = getY(event.getPos1().getLatitude());

        double plus = 0.02 * m_demands[event.getDemandID()];
        --m_demands[event.getDemandID()];

        draw_square(l_i, L_i, COLOR_TRUCK, plus);
    }
    for(auto& event : m_drone_deliveries)
    {
        double l_i = getX(event.getPos1().getLongitude());
        double L_i = getY(event.getPos1().getLatitude());

        std::string color = COLOR_DRONE1;
        if(event.getDroneID() != 0)
            color = COLOR_DRONE2;

        double plus = 0.03 * m_demands[event.getDemandID()];
        --m_demands[event.getDemandID()];

        draw_square(l_i, L_i, color, plus);
    }
}

void SolutionPreview::write_nodes()
{
    for(auto& node : m_nodes)
    {
        double x = getX(node.getLongitude()), y = getY(node.getLatitude());
        draw_node(x, y);
    }
}

void SolutionPreview::write_legend()
{
    double x_init = 0, y_init = -0.25;
    double x = x_init, y = y_init;
    std::string solution = "Solution pour cas ";
    for(unsigned int i = 0, n = m_solution.getIsValid().size(); i < n; ++i)
    {
        if (m_solution.getIsValid(i) == 1)
            solution += std::to_string(i) + ", ";
    }
    draw_node_with_label(x_init, y_init, "black", "right", solution);

    y -= 0.5;
    draw_square(x, y);
    draw_node_with_label(x + 0.5, y, "black", "right", "depot");
    y -= 0.5;
    draw_node(x, y);
    draw_node_with_label(x + 0.5, y, "black", "right", "node");

    y = y_init - 0.5;
    x = 7;
    draw_line(x, y, x + 0.5, y, COLOR_TRUCK);
    draw_node_with_label(x + 0.5, y, "black", "right", "truck");
    y -= 0.5;
    draw_line(x, y, x + 0.5, y, COLOR_DRONE1);
    draw_node_with_label(x + 0.5, y, "black", "right", "drone n°1");
    y -= 0.5;
    draw_line(x, y, x + 0.5, y, COLOR_DRONE2);
    draw_node_with_label(x + 0.5, y, "black", "right", "drone n°2");

    y = y_init - 0.5;
    x = 14;
    draw_square(x, y, "gray");
    draw_node_with_label(x + 0.5, y, "black", "right", "delivery");
    y -= 0.5;
    draw_node_with_label(x - 0.2, y, "gray", "right", "-");
    draw_node_with_label(x + 0.5, y, "black", "right", "drone lift off");
    y -= 0.5;
    draw_node_with_label(x - 0.25, y, "gray", "right", "+");
    draw_node_with_label(x + 0.5, y, "black", "right", "drone pick up");
}

void SolutionPreview::draw_node_with_label(double t_x_point, double t_y_point, const std::string &t_color, const std::string& t_place, const std::string& t_name)
{
    m_file << "  \\fill[" << t_color << "] (" << t_x_point << ", " << t_y_point << ") node [" << t_place << "]{" << t_name << "};\n";
}

void SolutionPreview::draw_arc(double t_x_first_point, double t_y_first_point,
                       double t_x_next_point, double t_y_next_point,
                       const std::string &t_color)
{
    m_file << "  \\draw[postaction={decorate}," << t_color << ", thick] (" << t_x_first_point << "," << t_y_first_point
           << ") -- (" << t_x_next_point << "," << t_y_next_point << ");\n";
}
