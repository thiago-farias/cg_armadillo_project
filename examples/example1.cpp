#include <fstream>
#include <armadillo>
#include <thread>

using namespace std;
using namespace arma;

// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html


bool intersect(const vec& origin, const vec& direction, const vec& center, double radius, vec& normal, vec& hit) {
	vec oc = origin - center;
	double a = dot(direction, direction);
	double b = 2.0*dot(direction, oc);
	double c = dot(oc, oc) - radius*radius;

	double delta = b*b - 4.0*a*c;
	if (delta < 0.0) {
		return false;
	}
	double t1 = (-b + sqrt(delta)) / (2.0*a);
	double t2 = (-b - sqrt(delta)) / (2.0*a);
	double t = (t1 < t2) ? t1 : t2;

	hit = origin + direction*t;
	normal = hit - center;
	return true;
}

int main() {
	vec centro_esfera = { 0.0, 0.0, 10.0 };
	vec origin = { 0.0, 0.0, 0.0 };
	double intensidade = 0.9;
	vec posicao_luz = { 0.0, 4.0, 4.0 };
	vec radiancia_luz = { intensidade, intensidade, intensidade };
	mat k;
	k << 1000.0 << 0.0 << 400.0 << endr
		<< 0.0 << -1000.0 << 300.0 << endr
		<< 0.0 << 0.0 << 1.0;
	mat invk = k.i();

	ofstream output;
	output.open("imagem.pgm");
	output << "P3" << endl;
	output << "800 600" << endl;
	output << "255" << endl;

	for (int linha = 0; linha < 600; linha++) {
		for (int coluna = 0; coluna < 800; coluna++) {
			vec r = { double(coluna), double(linha), 1.0 };
			vec normal, hit;
			
			r = invk * r;
			r *= 5.0 / r(2);
			if (intersect(origin, r, centro_esfera, 2.0, normal, hit)) {
				normal /= norm(normal);
				vec l = posicao_luz - hit;
				l /= norm(l);

				vec cor = { 255.0, 0.0, 0.0 };
				cor = (cor % radiancia_luz)*std::max(0.0, dot(normal, l));
				output << std::min(255, int(cor(0))) << " " 
					   << std::min(255, int(cor(1))) << " "
					   << std::min(255, int(cor(2))) << " ";
			} else {
				output << "255 255 255 ";
			}
		}
	}
	output.close();
	return 0;
}