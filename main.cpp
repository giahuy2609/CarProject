#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Lớp cơ sở mô phỏng các cảm biến trên ô tô
class Sensor {
protected:
    std::string sensorName;
    float currentData;

public:
    Sensor(std::string name) : sensorName(name), currentData(0.0) {}
    virtual void updateData(float newTick) = 0; // Hàm ảo cập nhật dữ liệu liên tục
    virtual bool checkAlert() = 0;             // Hàm ảo kiểm tra điều kiện an toàn
    
    std::string getName() const { return sensorName; }
    float getData() const { return currentData; }
};

// Cảm biến tốc độ xe
class SpeedSensor : public Sensor {
private:
    const float MAX_SPEED_LIMIT = 120.0; // Giới hạn tốc độ cho phép (km/h)
public:
    SpeedSensor() : Sensor("Vehicle Speed Sensor") {}

    void updateData(float speed) override {
        currentData = speed;
    }

    bool checkAlert() override {
        return currentData > MAX_SPEED_LIMIT;
    }
};

// Cảm biến nhiệt độ động cơ
class EngineTempSensor : public Sensor {
private:
    const float CRITICAL_TEMP = 105.0; // Nhiệt độ quá nhiệt nguy hiểm (độ C)
public:
    EngineTempSensor() : Sensor("Engine Temperature Sensor") {}

    void updateData(float temp) override {
        currentData = temp;
    }

    bool checkAlert() override {
        return currentData > CRITICAL_TEMP;
    }
};

// Hệ thống điều khiển trung tâm (ECU Dashboard)
class CarDashboardECU {
private:
    std::vector<Sensor*> sensors;
    bool systemSafetyStatus;

public:
    CarDashboardECU() : systemSafetyStatus(true) {}

    void registerSensor(Sensor* s) {
        sensors.push_back(s);
    }

    void processTelemetry(float speed, float temp) {
        sensors[0]->updateData(speed);
        sensors[1]->updateData(temp);
    }

    void displayStatus() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- FSOFT AUTOMOTIVE TELEMETRY DASHBOARD ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        
        systemSafetyStatus = true;

        for (Sensor* s : sensors) {
            std::cout << "-> " << std::left << std::setw(30) << s->getName() 
                      << ": " << std::fixed << std::setprecision(1) << s->getData();
            
            if (s->getName() == "Vehicle Speed Sensor") std::cout << " km/h";
            else std::cout << " 'C";

            if (s->checkAlert()) {
                std::cout << " [ WARNING: CRITICAL STATUS! ]";
                systemSafetyStatus = false;
            } else {
                std::cout << " [ OK ]";
            }
            std::cout << std::endl;
        }

        std::cout << "---------------------------------------------" << std::endl;
        if (systemSafetyStatus) {
            std::cout << "STATUS: SYSTEM SAFE. READY TO DRIVE." << std::endl;
        } else {
            std::cout << "STATUS: DANGER! PLEASE CHECK VEHICLE IMMEDIATELY." << std::endl;
        }
        std::cout << "=============================================\n" << std::endl;
    }
};

int main() {
    // Khởi tạo hệ thống điều khiển và cảm biến
    CarDashboardECU ecu;
    SpeedSensor speedSens;
    EngineTempSensor tempSens;

    ecu.registerSensor(&speedSens);
    ecu.registerSensor(&tempSens);

    // Kịch bản 1: Xe vận hành bình thường
    std::cout << "Simulating Normal Driving..." << std::endl;
    ecu.processTelemetry(80.5, 90.2); // Tốc độ 80.5km/h, Nhiệt độ 90.2 độ C
    ecu.displayStatus();

    // Kịch bản 2: Xe chạy quá tốc độ và động cơ quá nhiệt
    std::cout << "Simulating Dangerous Driving (Over speed & Overheat)..." << std::endl;
    ecu.processTelemetry(135.0, 108.5); // Tốc độ 135km/h, Nhiệt độ 108.5 độ C
    ecu.displayStatus();

    return 0;
}
