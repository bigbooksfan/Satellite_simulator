export module Compute_module;
import <vector>;

class Orbital_group {
private:            // nested classes
    class Satellite {
    private:        // nested classes
        struct Orbit {
            const unsigned int altitude = 0;                // высота над средним уровнем Земли, м
            const double inclination = 0.0;                 // наклонение, рад
            const double argument0 = 0.0;                   // орбитальный параметр в начальный момент времени, рад
            const double ascending_node_longitude0 = 0.0;   // Долгота восходящего узла в начальный момент времени, рад

            Orbit() = delete;
            Orbit(unsigned int alt, double i, double arg, double omega)
                : altitude(alt), inclination(i), argument0(arg), ascending_node_longitude0(omega) { }
        };
    private:        // fields
        const Orbit orbit;


    public:         // constructors
        Satellite() = delete;
        Satellite(unsigned int alt, double i, double arg, double omega)
            : orbit(Orbit(alt, i, arg, omega)) { }
    };

private:            // fields
    std::vector<Satellite> satellites;
    unsigned int number_of_satellites = 0;


};