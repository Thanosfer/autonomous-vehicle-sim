# Μεταγλωττιστής και flags
CXX = g++
CXXFLAGS = -std=c++17 -I./include -Wall -Wextra

# Φάκελοι
SRC_DIR = src
OBJ_DIR = obj

# Λίστα όλων των αρχείων πηγαίου κώδικα
SOURCES = $(SRC_DIR)/Position.cpp \
          $(SRC_DIR)/WorldObject.cpp \
          $(SRC_DIR)/StaticObject.cpp \
          $(SRC_DIR)/MovingObject.cpp \
          $(SRC_DIR)/GridWorld.cpp \
          $(SRC_DIR)/SensorReading.cpp \
          $(SRC_DIR)/Sensor.cpp \
          $(SRC_DIR)/LidarSensor.cpp \
          $(SRC_DIR)/RadarSensor.cpp \
          $(SRC_DIR)/CameraSensor.cpp \
          $(SRC_DIR)/SensorFusionEngine.cpp \
          $(SRC_DIR)/NavigationSystem.cpp \
          $(SRC_DIR)/SelfDrivingCar.cpp \
          $(SRC_DIR)/TrafficLight.cpp \
          $(SRC_DIR)/TrafficSign.cpp \
          $(SRC_DIR)/Visualization.cpp \
          $(SRC_DIR)/CommandLineParser.cpp \
          $(SRC_DIR)/main.cpp

# Μετατροπή .cpp σε .o
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Εκτελέσιμο
EXEC = oopproj_2025

# Κυριός στόχος
all: $(EXEC)

# Σύνδεση αντικειμένων για δημιουργία εκτελέσιμου
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Μεταγλώττιση κάθε .cpp σε .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Δημιουργία φακέλου obj αν δεν υπάρχει
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Καθαρισμός
clean:
	rm -rf $(OBJ_DIR) $(EXEC)

# Εκτέλεση με βασικές παραμέτρους
run: $(EXEC)
	./$(EXEC) --gps 10 10 20 20 30 30

# Εκτέλεση με παραδειγματικές παραμέτρους
run-example: $(EXEC)
	./$(EXEC) --seed 123 --dimX 30 --dimY 30 --numMovingCars 3 \
	          --numTrafficLights 2 --gps 5 5 15 15 25 25

# Βοήθεια
help: $(EXEC)
	./$(EXEC) --help

.PHONY: all clean run run-example help