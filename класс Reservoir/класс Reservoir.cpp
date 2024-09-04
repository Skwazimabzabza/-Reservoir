#include <iostream>
#include <fstream>
#include <cstring>

class Reservoir {
private:
    char* name;
    double width;
    double length;
    double maxDepth;
    std::string type;

public:
    explicit Reservoir() : name(nullptr), width(0), length(0), maxDepth(0), type("") {}

    explicit Reservoir(const char* name, double width, double length, double maxDepth, const std::string& type)
        : width(width), length(length), maxDepth(maxDepth), type(type) {
        this->name = new char[strlen(name) + 1];
        for (size_t i = 0; i <= strlen(name); ++i) {
            this->name[i] = name[i];
        }
    }
    Reservoir(const Reservoir& other) : width(other.width), length(other.length), maxDepth(other.maxDepth), type(other.type) {
        name = new char[strlen(other.name) + 1];
        for (size_t i = 0; i <= strlen(other.name); ++i) {
            name[i] = other.name[i];
        }
    }

    Reservoir& operator=(const Reservoir& other) {
        if (this == &other) return *this;
        delete[] name;
        width = other.width;
        length = other.length;
        maxDepth = other.maxDepth;
        type = other.type;
        name = new char[strlen(other.name) + 1];
        for (size_t i = 0; i <= strlen(other.name); ++i) {
            name[i] = other.name[i];
        }
        return *this;
    }

    // Деструктор
    ~Reservoir() {
        delete[] name;
    }

    void setName(const char* name) {
        delete[] this->name;
        this->name = new char[strlen(name) + 1];
        for (size_t i = 0; i <= strlen(name); ++i) {
            this->name[i] = name[i];
        }
    }

    const char* getName() const {
        return name;
    }

    void setWidth(double width) {
        this->width = width;
    }

    double getWidth() const {
        return width;
    }

    void setLength(double length) {
        this->length = length;
    }

    double getLength() const {
        return length;
    }

    void setMaxDepth(double maxDepth) {
        this->maxDepth = maxDepth;
    }

    double getMaxDepth() const {
        return maxDepth;
    }

    void setType(const std::string& type) {
        this->type = type;
    }

    const std::string& getType() const {
        return type;
    }

    double calculateVolume() const {
        return width * length * maxDepth;
    }

    double calculateSurfaceArea() const {
        return width * length;
    }

    bool isSameType(const Reservoir& other) const {
        return type == other.type;
    }

    bool hasLargerSurfaceArea(const Reservoir& other) const {
        if (isSameType(other)) {
            return calculateSurfaceArea() > other.calculateSurfaceArea();
        }
        return false;
    }

    void display() const {
        std::cout << "Название: " << (name ? name : "Unnamed") << std::endl;
        std::cout << "Ширина: " << width << std::endl;
        std::cout << "Длина: " << length << std::endl;
        std::cout << "Максимальная глубина: " << maxDepth << std::endl;
        std::cout << "Тип: " << type << std::endl;
        std::cout << "Объём: " << calculateVolume() << std::endl;
        std::cout << "Площадь поверхности: " << calculateSurfaceArea() << std::endl;
    }
};

class ReservoirManager {
private:
    Reservoir* reservoirs;
    int count;
    int capacity;

    void resize() {
        capacity *= 2;
        Reservoir* newReservoirs = new Reservoir[capacity];
        for (int i = 0; i < count; ++i) {
            newReservoirs[i] = reservoirs[i];
        }
        delete[] reservoirs;
        reservoirs = newReservoirs;
    }

public:
    ReservoirManager() : count(0), capacity(10) {
        reservoirs = new Reservoir[capacity];
    }

    ~ReservoirManager() {
        delete[] reservoirs;
    }

    void addReservoir(const Reservoir& reservoir) {
        if (count >= capacity) {
            resize();
        }
        reservoirs[count++] = reservoir;
    }

    void removeReservoir(int index) {
        if (index < 0 || index >= count) return;
        for (int i = index; i < count - 1; ++i) {
            reservoirs[i] = reservoirs[i + 1];
        }
        --count;
    }

    void displayAll() const {
        for (int i = 0; i < count; ++i) {
            reservoirs[i].display();
            std::cout << "--------------------------" << std::endl;
        }
    }

    // Запись информации в текстовый файл
    void writeToFile(const std::string& filename) const {
        std::ofstream ofs(filename);
        if (ofs.is_open()) {
            for (int i = 0; i < count; ++i) {
                ofs << reservoirs[i].getName() << std::endl;
                ofs << reservoirs[i].getWidth() << std::endl;
                ofs << reservoirs[i].getLength() << std::endl;
                ofs << reservoirs[i].getMaxDepth() << std::endl;
                ofs << reservoirs[i].getType() << std::endl;
            }
            ofs.close();
        }
    }

    void writeToBinaryFile(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::binary);
        if (ofs.is_open()) {
            for (int i = 0; i < count; ++i) {
                size_t nameLength = std::strlen(reservoirs[i].getName());
                ofs.write((char*)&nameLength, sizeof(nameLength));
                ofs.write(reservoirs[i].getName(), nameLength);
                double width = reservoirs[i].getWidth();
                double length = reservoirs[i].getLength();
                double maxDepth = reservoirs[i].getMaxDepth();
                ofs.write((char*)&width, sizeof(width));
                ofs.write((char*)&length, sizeof(length));
                ofs.write((char*)&maxDepth, sizeof(maxDepth));
                size_t typeLength = reservoirs[i].getType().length();
                ofs.write((char*)&typeLength, sizeof(typeLength));
                ofs.write(reservoirs[i].getType().c_str(), typeLength);
            }
            ofs.close();
        }
    }
};

int main() {

    setlocale(LC_ALL, "Ru");

    ReservoirManager manager;

    Reservoir r1("Озеро Верхнее", 80, 160, 10, "озеро");
    Reservoir r2("Тихий океан", 200, 400, 30, "океан");
    Reservoir r3("Чёрное море", 100, 200, 20, "море");

    manager.addReservoir(r1);
    manager.addReservoir(r2);
    manager.addReservoir(r3);

    std::cout << "Все водоёмы:" << std::endl;
    manager.displayAll();

    manager.writeToFile("reservoirs.txt");

    manager.writeToBinaryFile("reservoirs.bin");

    manager.removeReservoir(1);

    std::cout << "Водоемы после удаления:" << std::endl;
    manager.displayAll();

    return 0;
}