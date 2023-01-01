#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
using namespace std;

//Data structure we will be needing
map<string, int> CityIndexes; //given name of city, returns its index
vector<string> Index_to_City; //given index, returns name of city
vector<vector<pair<int,int>>> time_list; //adjacency list, weights = time
vector<vector<pair<int,int>>> dist_list; //adjacency list, weights = distance
vector<vector<int>> time_mat; //adjacency matrix, weights = time
vector<vector<int>> dist_mat; //adjacency matrix, weights = distance

//gives the position of each city
#define FILE_FOR_COORDINATES "cities.txt"
//gives the input graph
#define FILE_FOR_EDGES "Input_Graph.txt"
#define INF 1000000000

//SFML objects initalized
vector<pair<sf::CircleShape,sf::Text>> cities; //contains each node of each city
sf::RenderWindow window; //for graphical display
sf::Texture mapOfPak; //Will load map from .jpg file
sf::Sprite spritemap; //will be used to draw the map
sf::Font font; //will load font from file

//SFML MACROS
#define DEFAULT_CIR_COLOR sf::Color::Magenta
#define DEFAULT_TEXT_COLOR sf::Color::Black
#define DEFAULT_LINE_COLOR sf::Color::Black
#define SPECIAL_CIR_COLOR sf::Color::Green
#define SPECIAL_CIR_COLOR_2 sf::Color::Cyan
#define SPECIAL_TEXT_COLOR sf::Color::Green
#define SPECIAL_TEXT_COLOR_2 sf::Color::Cyan
#define SPECIAL_LINE_COLOR sf::Color::Red
#define WINDOW_X 1000
#define WINDOW_Y 1000

//Converts Latitude and Longitude into X,Y coordinates for Map
void convertCoordinates(double &x,double &y){
    x = ((514-219)/(74.3587-67.0011))*(x-67.0011) + 219;
    y = ((522-261)/(24.8607-31.5204))*(y-31.5204) + 261;
}

//centralizes the origin of the given SFML object
template<class T> void centralize(T &obj){
    auto rect = obj.getGlobalBounds();
    obj.setOrigin(rect.left+rect.width/2, rect.top+rect.height/2);
}

//Returns a reference to either the Time Matrix OR Distance Matrix
vector<vector<int>>& adj_Mat(bool is_time){
    if(is_time) return time_mat;
    else return dist_mat;
};

//Returns a reference to either the Time List OR Distance List
vector<vector<pair<int,int>>>& adj_List(bool is_time){
    if(is_time) return time_list;
    else return dist_list;
};

//Edge for Kruskall
struct Edge {
    int u, v, weight; //the 2 nodes and the weight of the edge

    //CONSTRUCTOR
    Edge(int a, int b, int c): u(a), v(b), weight(c){}

    //COMPARATOR
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }

    //DISJOINT SET UNION
    static vector<int> parent;
    static vector<int> size;

    //initalizes each node to its own set
    static void make_set(int v){
        parent[v] = v;
        size[v] = 1;
    }

    //returns parent of a node, performs Path compression
    static int find_set(int v){
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    }

    //unites 2 disjoint sets.
    static void union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (size[a] < size[b])
                swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
    }
}; vector<int> Edge::parent;
vector<int> Edge::size;

//ALGORITHM THAT MINIMUM SPANNING TREE
vector<Edge> Kruskal(bool time){
    vector<vector<pair<int,int>>> &list = adj_List(time);
    int n = list.size();

    //On first call, this will be needed
    if(Edge::parent.empty()) Edge::parent.resize(n);
    if(Edge::size.empty()) Edge::size.resize(n);

    //stores edges
    vector<Edge> edges;

    //stores minimum spanning tree
    vector<Edge> result;

    int cost = 0;

    //Extract the edges from known Adjacency List
    for (int i = 1; i < n; i++)
        Edge::make_set(i);

    for(int i = 1; i < n; i++){
        for(auto &j: list[i]){
            edges.push_back(Edge(i, j.first, j.second));
        }
    }

    //Sort the Edges w.r.t their weights.
    sort(edges.begin(), edges.end());

    //One by one keep adding the minimum weight edges until all nodes are visited
    for (Edge e : edges) {
        if (Edge::find_set(e.u) != Edge::find_set(e.v)) {
            cost += e.weight;
            result.push_back(e);
            Edge::union_sets(e.u, e.v);
        }
    }

    //Print the edges in the minimum spanning tree
    for(int i = 1; i <= result.size(); i++){
        int a = result[i-1].u;
        int b = result[i-1].v;
        int w = result[i-1].weight;

        cout << setw(40) << Index_to_City[a] + "-" + Index_to_City[b] + " : " + to_string(w);
        cout << (time ? " mins" : " km");
        if(i%3 == 0) cout << endl;
    }

    //Print the Total cost of the tree
    cout << "\nMinimum ";
    if(time) cout << "Time: ";
    else cout << "Distance ";
    cout << cost << (time ? " mins" : " km") << endl;
    return result;
}

//Initializes SFML Window and draws the Map of Pakistan
bool Initialize_Window(){
    //Opens a new window
    window.create(sf::VideoMode(WINDOW_X, WINDOW_Y), "My window");
    window.clear();

    //Map is loaded
    if( !(mapOfPak.loadFromFile("Pakistan_Map_Province_Wise.jpg"))){
        return 0;
    } spritemap.setTexture(mapOfPak);

    //Draws The Map
    window.draw(spritemap);

    //Draws the edges between cities
    for(int i=1;i<cities.size();i++){
        for(auto p: dist_list[i]){
            int j = p.first;
            int w = p.second;
            if(w<INF){
                sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, DEFAULT_LINE_COLOR}, {{circ2.getPosition().x,circ2.getPosition().y}, DEFAULT_LINE_COLOR}};
                window.draw(line, 2,sf::Lines);
            }
        }
    }

    //Draws the cities and their names
    for(int i=1;i<cities.size();i++){
        window.draw(cities[i].first);
        window.draw(cities[i].second);
    }
    return 1;
}

//Resets the cities to default colors after any changes
void Reset_Cities(){
    for(int i = 1; i < cities.size(); i++){
        cities[i].first.setFillColor(DEFAULT_CIR_COLOR);
        cities[i].second.setFillColor(DEFAULT_TEXT_COLOR);
    }
}

//Breadth First Search - Complete Visualization
void BFS(int i, vector<vector<pair<int,int>>> &list){
    if(!Initialize_Window()) {
        cout << "Sorry map couldn't load. Try again\n";
        return;
    }

    int n = list.size();

    //Draws a city node and its name
    auto draw_city = [&](int i){
        window.draw(cities[i].first);
        window.draw(cities[i].second);
    };

    //gives the visited nodes a different color
    auto visited_city = [&](int i){
        cities[i].first.setFillColor(SPECIAL_CIR_COLOR);
        cities[i].second.setFillColor(SPECIAL_TEXT_COLOR);
    };

    //Gives the city at the front of the queue a special color
    auto queue_top = [&](int i){
        cities[i].first.setFillColor(SPECIAL_CIR_COLOR_2);
        cities[i].second.setFillColor(SPECIAL_TEXT_COLOR_2);
    };

    //draws line btw given points
    auto draw_line = [&](int i, int j, sf::Color col){
        sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
        sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, col}, {{circ2.getPosition().x,circ2.getPosition().y}, col}};
        window.draw(line, 2,sf::Lines);
    };

    vector<vector<bool>> visited_edges(n, vector<bool>(n, 0)); //keeps track of edges
    vector<bool> visited(n, 0); //keeps track of nodes
    visited[0] = 1; //empty node
    visited[i] = 1; //starting city is i

    //BFS is done with a queue
    queue<int> q;
    q.push(i);
    queue_top(i);
    draw_city(i);
    window.display();

    double zoomFactor = 1;
    bool next = 1;
    int city;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
                zoomFactor += 0.1;
            }if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
                zoomFactor -= 0.1;
            }

            //Pressing right arrow key moves the BFS one step forward
            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) continue;
            if(q.empty()) continue;

            //if true, pop()
            if(next){
                city = q.front();
                q.pop();
                next = 0;
            }

            //visits next neighbor of city
            bool none = 1;
            for(int k = 0; k < list[city].size(); k++){
                pair<int,int> u = list[city][k];
                if(!visited[u.first]){
                    none = 0;
                    visited[u.first] = 1;
                    visited_edges[city][u.first] = 1;
                    visited_edges[u.first][city] = 1;
                    visited_city(u.first);
                    q.push(u.first);
                    break;
                }
            }

            //tells whether the parent city needs to change or not
            if(none) {
                next = 1;
                visited_city(city);
                if(q.size()) queue_top(q.front());
            }
        }

        //sets the zoom view
        sf::View view;
        view.setViewport(sf::FloatRect(0.f, 0.f, zoomFactor * 1.f, zoomFactor * 1.f));
        window.clear();
        window.setView(view);
        window.draw(spritemap); //draws map

        //displays the graph
        for(int i=1;i<n;i++){
            for(int k = 0; k < list[i].size(); k++){
                pair<int,int> p = list[i][k];
                int j = p.first;
                int w = p.second;
                if(w<INF){
                    sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                    bool same = !visited_edges[i][j];
                    draw_line(i, j, (same? DEFAULT_LINE_COLOR : SPECIAL_LINE_COLOR));
                }
            }
        }

        for(int i=1;i<n;i++){
            draw_city(i);
        }
        window.display();

    }

    //resets colors of the cities
    Reset_Cities();
}

//ALGORITHM TO FIND SHORTEST PATH BETWEEN ALL NODES
vector<vector<int>> Floyd_Warshall(bool time){
    // Number of vertices and edges in the graph
    vector<vector<int>> &adj = adj_Mat(time);
    int n = adj.size();

    // Initialize the distance matrix
    vector<vector<int>> dist(adj);

    // Floyd-Warshall algorithm
    for (int k = 1; k < n; k++)
        for (int i = 1; i < n; i++)
            for (int j = 1; j < n; j++)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    // Print the shortest paths
    /*
    cout << "__Shortest paths:__ \n";
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            cout << Index_to_City[i] << " - " << Index_to_City[j] << " : ";
            if (dist[i][j] == INF)
                cout << "NONE ";
            else
                cout << dist[i][j] << (time ? " mins" : " km");
            cout << endl;
        } cout << endl;
    } cout << endl;
    */
    return dist;
}

//DISPLAYS MINIMUM SPANNING TREE
void Display_Kruskal(bool time){
    if(!Initialize_Window()){
        "Failed!! Try again.\n";
        return;
    };

    int n = adj_List(time).size();
    std::vector<Edge> KruskalResult = Kruskal(time);

    //Makes an adjacency matrix from the edges contained in MST
    std::vector<std::vector<int>> KruskalAdjMat(n,std::vector<int>(n,INF));
    for(int i=0;i<KruskalResult.size();i++){
        int a = KruskalResult[i].u;
        int b = KruskalResult[i].v;
        int c = KruskalResult[i].weight;
        KruskalAdjMat[a][b] = c;
        KruskalAdjMat[b][a] = c;
    }

    //draws the MST
    for(int i=1;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(KruskalAdjMat[i][j]<INF){
                sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, SPECIAL_LINE_COLOR}, {{circ2.getPosition().x,circ2.getPosition().y}, SPECIAL_LINE_COLOR}};
                window.draw(line, 2,sf::Lines);
            }
        }
    }

    window.display();
    double zoomFactor = 1;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            //ZOOM FEATURE
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
                zoomFactor += 0.1;
            }if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
                zoomFactor -= 0.1;
            }
        }
        sf::View view;

        view.setViewport(sf::FloatRect(0.f, 0.f, zoomFactor * 1.f, zoomFactor * 1.f));
        window.clear();
        window.setView(view);
        window.draw(spritemap);

        //DISPLAYS GRAPH
        for(int i=1;i<n;i++){
            for(int j=i+1;j<n;j++){
                if(KruskalAdjMat[i][j] < INF){
                    sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                    sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, SPECIAL_LINE_COLOR}, {{circ2.getPosition().x,circ2.getPosition().y}, SPECIAL_LINE_COLOR}};
                    window.draw(line, 2,sf::Lines);
                } else if(adj_Mat(time)[i][j] < INF){
                    sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                    sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, DEFAULT_LINE_COLOR}, {{circ2.getPosition().x,circ2.getPosition().y}, DEFAULT_LINE_COLOR}};
                    window.draw(line, 2,sf::Lines);
                }
            }
        }

        for(int i=1;i<n;i++){
            window.draw(cities[i].first);
            window.draw(cities[i].second);
        }

        window.display();
    }
}

//DISPLAYS THE MAP ONLY
void Display_Graph(){
    if(!Initialize_Window()){
        "Failed!! Try again.\n";
        return;
    };

    int n = cities.size();
    window.display();
    double zoomFactor = 1;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
                zoomFactor += 0.1;
            }if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
                zoomFactor -= 0.1;
            }
        }
        sf::View view;

        // cout<<zoomFactor<<endl;
        view.setViewport(sf::FloatRect(0.f, 0.f, zoomFactor * 1.f, zoomFactor * 1.f));
        window.clear();
        window.setView(view);
        window.draw(spritemap);

        //Draws the edges btw cities
        for(int i=1;i<n;i++){
            for(int j=i+1;j<n;j++){
                if(dist_mat[i][j]<INF){
                    sf::CircleShape circ1 = cities[i].first,circ2 = cities[j].first;
                    sf::Vertex line[] = {{{circ1.getPosition().x,circ1.getPosition().y}, sf::Color::Black}, {{circ2.getPosition().x,circ2.getPosition().y}, sf::Color::Black}};
                    window.draw(line, 2,sf::Lines);
                }
            }
        }

        //Draws the cities and their names
        for(int i=1;i<n;i++){
            window.draw(cities[i].first);
            window.draw(cities[i].second);
        }

        window.display();
    }
}

//Takes Edges as Input i.e. Graph as Input
void Take_Input_Graph(){
    ifstream in;
    in.open(FILE_FOR_EDGES);

    string s1, s2;
    int t, d;
    int n = cities.size();

    //Resize the adjacency lists and matrices
    time_list.resize(n);
    dist_list.resize(n);
    time_mat.resize(n, vector<int>(n, INF));
    dist_mat.resize(n, vector<int>(n, INF));

    //Update the adjacency matrices and lists
    while(in >> s1 >> s2 >> t >> d){
        int i = CityIndexes[s1];
        int j = CityIndexes[s2];

        if(time_mat[i][j] == INF) time_mat[i][j] = t;
        else continue;

        time_mat[j][i] = t;
        dist_mat[j][i] = d;
        dist_mat[i][j] = d;

        auto update2 = [&](int a, int b){
            time_list[a].push_back({b,t});
            dist_list[a].push_back({b,d});
        };

        update2(i, j);
        update2(j, i);
    } in.close();

    //0 weight btw same node
    for(int i = 0; i < n; i++) {
        time_mat[i][i] = 0;
        dist_mat[i][i] = 0;
    }
}

//Initializes the City Nodes and stores them - only needed to run once
void Initialize_Cities(){
    if (!font.loadFromFile("solidworks gdt.ttf"))
    {
        cout<<"Font not found";
        // error...
    }

    ifstream in;
    in.open(FILE_FOR_COORDINATES);

    int n;                      // number of cities
    in >> n;

    //resize the arrays
    Index_to_City.resize(n+1);
    cities.resize(n+1);

    //Circle / Node / City
    sf::CircleShape circle;
    circle.setRadius(3.f);
    circle.setFillColor(DEFAULT_CIR_COLOR);
    centralize(circle);

    //Name of City
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(10);
    text.setFillColor(DEFAULT_TEXT_COLOR);

    //Takes as input latitude and longitude of a city, converts it into x,y coordinates for the map and stores them
    for(int i=1;i<=n;i++){
        double x,y;
        string city;
        in >> city >> x >> y;

        CityIndexes[city] = i;
        Index_to_City[i] = city;

        convertCoordinates(x,y);

        circle.setPosition(x,y);
        text.setPosition(circle.getPosition().x-2*city.size(),circle.getPosition().y+5);
        text.setString(city);
        cities[i].first = circle;
        cities[i].second = text;
    } in.close();
}

int main(){
    Initialize_Cities();
    Take_Input_Graph();

    int choice;
    cout << "__WELCOME TO OUR PROJECT__\n\n";
    cout << "We are a railway company making a feasibility report for laying our rail network in the country.\n";
    do{
        cout << "\nHow would you like to proceed? \n"
            << "1.) Display The Map with All the Possibilities\n"
            << "2.) Display the Minimum Spanning Rail Network\n"
            << "3.) Visualize BFS\n"
            << "4.) Find Shortest Path Btw 2 Cities\n"
            << "5.) Exit\n"
            << "Your choice: ";
        cin >> choice;

        if(choice == 1){
            cout << "\nNote: you can press ESC key to exit the display window!\n";
            Display_Graph();
        }
        else if(choice == 2){
            string time_or_dist;

            cout << "\nNote that once the window opens, You can press O/P to zoom in/out, ESC to Exit\n";
            do{
                cout << "What do you want to minimize, Time or Distance? (t/d)";
                cin >> time_or_dist;

            } while(time_or_dist != "t" && time_or_dist != "d" && time_or_dist != "T" && time_or_dist != "D");

            Display_Kruskal(time_or_dist == "t" || time_or_dist == "T");
        }
        else if(choice == 3){
            string city, cont;

            cout <<"\nNote: Right Arrow Key to move forward one step, O/P to zoom, ESC to exit";
            //DISPLAY ALL THE CITIES SO USER CAN CHOOSE ONE
            cout << "\nCITIES IN THE MAP\n";
            int i = 0;
            for(auto &c: Index_to_City)
            {
                cout << setw(20) << c;
                if(i++ % 5 == 0) cout << endl;
            }

            do{
                do{
                    cout << "\nChoose Starting City: ";
                    cin >> city;

                } while(CityIndexes.count(city) == 0); //keeps asking if city is not in database

                BFS(CityIndexes[city], dist_list);
                cout << "Do you want to continue with BFS (y/n)?  ";
                cin >> cont;

            } while(cont == "y" || cont == "Y");
        }
        else if(choice == 4){
            string time_or_dist;
            bool is_time;

            do{
                cout << "What do you want to minimize, Time or Distance (t/d)?  ";
                cin >> time_or_dist;

            } while(time_or_dist != "t" && time_or_dist != "d" && time_or_dist != "T" && time_or_dist != "D");

            is_time = (time_or_dist == "t" || time_or_dist == "T");
            vector<vector<int>> dist = Floyd_Warshall(is_time);

            //DISPLAY ALL THE CITIES SO USER CAN CHOOSE ONE
            cout << "\nCITIES IN THE MAP\n";
            int i = 0;
            for(auto &c: Index_to_City)
            {
                cout << setw(20) << c;
                if(i++ % 5 == 0) cout << endl;
            }

            string city1, city2, cont;
            do{
                cout << "\nEnter city 1: ";
                cin >> city1;
                while(CityIndexes.count(city1) == 0) //if city is not in database, keep asking
                {
                    cout << "Please enter valid city: ";
                    cin >> city1;
                }

                cout << "Enter city 2: ";
                cin >> city2;
                while(CityIndexes.count(city2) == 0) //if city is not in database, keep asking
                {
                    cout << "Please enter valid city: ";
                    cin >> city2;
                }

                cout << "Min " << (is_time?"time: ":"distance: ") << dist[CityIndexes[city1]][CityIndexes[city2]] << (is_time ? " mins" : " km");
                cout << "\nDo you want to know more (y/n)?  ";
                cin >> cont;
            } while(cont == "y" || cont == "Y"); //continues if "Y"
        }
        else if(choice != 5){
            cout << "Invalid Input\n";
        }

    } while(choice != 5); // Exit = 5

    cout << "You chose to exit the program.";
    return 0;
}
