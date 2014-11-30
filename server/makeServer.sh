echo "Building Server...."
echo "g++ -c tictac.cpp"
g++ -g -c tictac.cpp
echo "g++ -c components.cpp"
g++ -g -c components.cpp
echo "g++ -c smartserver.cpp"
g++ -g -c smartserver.cpp
echo ""
echo ""
echo "Object files built. I should be linking now...."
echo "g++ -o smartserver tictac.o components.o smartserver.o -lprotobuf -lpthread"
g++ -g -o smartserver tictac.o components.o smartserver.o -lprotobuf -lpthread
echo ""
echo ""
echo "smartserver built"