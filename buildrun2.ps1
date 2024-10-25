cls

rm -r -fo ./build | Out-null
mkdir ./build | Out-null

g++ -c courses.cpp -o ./build/courses.o -Wall -Wextra -O3
g++ -c lists.cpp -o ./build/lists.o -Wall -Wextra -O3
g++ -c persons.cpp -o ./build/persons.o -Wall -Wextra -O3
g++ -c main.cpp -o ./build/main.o -Wall -Wextra -O3
g++  -Wall -Wextra -O3 -o CourseManagementSystem.exe ./build/persons.o ./build/lists.o ./build/courses.o ./build/main.o

./CourseManagementSystem.exe
pause