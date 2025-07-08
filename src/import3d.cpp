#include "import3d.h"

using namespace std;

Mesh importObj(string filename)
{
	string line;
	string inxStr;
	ifstream objfile("F:/Creative raw/repos/barbershop/content/obj/cube.obj");
	Mesh result;
	if (objfile.is_open())
	{
		while (getline(objfile, line))
		{
			if (line._Starts_with("v "))
			{
				istringstream substr(line.substr(2));
				double xin, yin, zin;
				substr >> xin >> yin >> zin;
				result.addVertex(Position3d(xin, yin, zin));
			}
			if (line._Starts_with("f "))
			{
				istringstream ss(line.substr(2));
				string token;
				vector<int> faceIndices;

				while (ss >> token)
				{
					istringstream tokenStream(token);
					string vStr;
					getline(tokenStream, vStr, '/');  // Get the position index only
					int vIndex = stoi(vStr) - 1;      // OBJ is 1-based; convert to 0-based
					faceIndices.push_back(vIndex);
				}

				result.addFace(faceIndices);  // You may want to triangulate here

			}
		}
		return result;
	}
	std::cout << "Import error" << endl;;
	return result;
}
