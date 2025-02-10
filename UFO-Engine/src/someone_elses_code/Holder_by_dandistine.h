//-------------------------------------------------//
//             File made by Dandistine.            //
//-------------------------------------------------//

// Type your code here, or load an example.
#include <iostream>
#include <type_traits>
#include <memory>
#include <map>
#include <typeindex>

struct Holder {
    struct DataInterface {
        virtual ~DataInterface() = default;
    };

    template<typename T>
    struct Field : public DataInterface {
        T value;
        explicit Field(const T& val) : value{val} {};
    };

    struct Assigner {
        Holder& parent;
        const std::string& idx;

        // General template for inserting data into the Holder
        template<typename T>
        Assigner& operator=(const T&& value) {
            parent.fields[typeid(T)].emplace(idx, std::make_unique<Field<T>>(value));// = std::make_unique<T>(value);
            return *this;
        }

        // Special overload for (maybe) making holder["key"] = "some string literal"; work correctly
        // I think it works but I'm not 100%
        Assigner& operator=(const char * const value) {
            parent.fields[typeid(std::string)].emplace(idx, std::make_unique<Field<std::string>>(value));
            return *this;
        }

        // Templated function to retireve a specific value of a given type.
        // This does assume that the specified index was actually filled in with this value
        template<typename T>
        T& as() {
            DataInterface* ptr = parent.fields[typeid(T)][idx].get();
            return dynamic_cast<Field<T>*>(ptr)->value;
        }
    };

    // Create and return an Assigner that can interact with the privately held data
    // using the "correct" types
    Assigner operator[](const std::string& idx) {
        return Assigner{*this, idx};
    }

private:
    std::map<std::type_index, std::map<std::string, std::unique_ptr<DataInterface>>> fields;

    friend Assigner;
};


int main() noexcept {
    Holder h;
    h["x"] = 7;
    h["y"] = std::string{"19"};

    // Notice that this will become a std::string and not stored as a const char*
    h["z"] = "dddddd";

    // Here is an intersting quirk that could be fixed I think
    // Assigning the same index with a different type does *not* clear it from the other type's storage
    h["x"] = std::string{"42"};

    std::cout << h["x"].as<int>() << ":" << h["y"].as<std::string>() << ":" << h["z"].as<std::string>() << std::endl;

    std::cout << "h[\"x\"].as<int>() == " << h["x"].as<int>() << std::endl;
    std::cout << "h[\"x\"].as<std::string>() == " << h["x"].as<std::string>() << std::endl;
    return 0;
}
