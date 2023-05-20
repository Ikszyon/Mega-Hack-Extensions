#pragma once
#define MH_DLL __declspec(dllimport)
#define MH_CALL __stdcall
#define MH_FWD 1
#define MH_BWD 0

#ifndef MH_BOX_MAX
#define MH_BOX_MAX 128
#endif

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

namespace MegaHackExt {

    struct Colour {
        unsigned char r, g, b;

        std::string toHexString() {
            std::stringstream SStr;

            SStr << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)r
                 << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)g
                 << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)b;

            return SStr.str();
        }

        void fromHexString(std::string HexColor) {
            if(HexColor.length() != 6) {
                HexColor = "000000";
            }

            std::vector<uint8_t> VectorColor;

            for(int i = 0; i < HexColor.length(); i += 2) {
                unsigned int x;
                std::stringstream SStr;

                SStr << std::hex << HexColor.substr(i, 2);
                SStr >> x;
                VectorColor.push_back(x);
            }

            r = VectorColor[0];
            g = VectorColor[1];
            b = VectorColor[2];
        }
    };

    namespace Client {
        MH_DLL void setModEnabled(const char* str, bool b);
        MH_DLL bool getModEnabled(const char* str);

        MH_DLL void commit(void* ptr);
    }

    class Window {
       public:
        MH_DLL Window() = delete;
        MH_DLL Window(const Window&) = delete;
        MH_DLL Window& operator=(const Window&) = delete;
        MH_DLL ~Window() = delete;

        MH_DLL static Window* Create(const char* title);

        MH_DLL void add(void* obj);

        void addElements(std::vector<void*> elements, bool auto_commit = true) {
            for(int i = elements.size() - 1; i >= 0; i--) {
                this->add(elements[i]);
            }
            if(auto_commit) {
                Client::commit(this);
            }
        }

        static Window* CreateEx(const char* title, std::vector<void*> elements, bool auto_commit = true) {
            Window* window = Create(title);
            window->addElements(elements, auto_commit);
            return window;
        }
    };

    class MH_DLL HorizontalLayout {
       public:
        HorizontalLayout() = delete;
        HorizontalLayout(const HorizontalLayout&) = delete;
        HorizontalLayout& operator=(const HorizontalLayout&) = delete;
        ~HorizontalLayout() = delete;

        static HorizontalLayout* Create(void* left, void* right);
    };

    class MH_DLL Label {
       public:
        Label() = delete;
        Label(const Label&) = delete;
        Label& operator=(const Label&) = delete;
        ~Label() = delete;

        static Label* Create(const char* str);

        void set(const char* str);
    };

    class Button {
        typedef void(MH_CALL* Callback)(Button*);

       public:
        MH_DLL Button() = delete;
        MH_DLL Button(const Button&) = delete;
        MH_DLL Button& operator=(const Button&) = delete;
        MH_DLL ~Button() = delete;

        MH_DLL static Button* Create(const char* str);

        MH_DLL void setCallback(Callback callback);

        static Button* CreateEx(const char* str, Callback callback) {
            Button* obj = Create(str);
            obj->setCallback(callback);
            return obj;
        }
    };

    class CheckBox {
        typedef void(MH_CALL* Callback)(CheckBox*, bool);

       public:
        MH_DLL CheckBox() = delete;
        MH_DLL CheckBox(const CheckBox&) = delete;
        MH_DLL CheckBox& operator=(const CheckBox&) = delete;
        MH_DLL ~CheckBox() = delete;

        MH_DLL static CheckBox* Create(const char* str);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void set(bool b, bool trigger_callback = true);
        MH_DLL bool get() const;

        static CheckBox* CreateEx(const char* str, bool b, bool trigger_callback, Callback callback) {
            CheckBox* obj = Create(str);
            obj->setCallback(callback);
            obj->set(b, trigger_callback);
            return obj;
        }
    };

    class ComboBox {
        typedef void(MH_CALL* Callback)(ComboBox*, int, const char*);

       public:
        MH_DLL ComboBox() = delete;
        MH_DLL ComboBox(const ComboBox&) = delete;
        MH_DLL ComboBox& operator=(const ComboBox&) = delete;
        MH_DLL ~ComboBox() = delete;

        MH_DLL static ComboBox* Create(const char* prefix, const char* suffix);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void setValues(const char* strs[], bool trigger_callback = true);

        MH_DLL void set(int i, bool trigger_callback = true);
        MH_DLL int get() const;

        void refresh(bool trigger_callback = true) {
            this->set(this->get(), trigger_callback);
        }

        void strValues(std::vector<std::string> vector, bool direction = MH_FWD) {
            const char* strs[MH_BOX_MAX];
            if(direction) {
                for(int i = vector.size() - 1; i >= 0; i--) {
                    strs[vector.size() - 1 - i] = vector[i].c_str();
                }
            } else {
                for(int i = 0; i < vector.size(); i++) {
                    strs[i] = vector[i].c_str();
                }
            }
            strs[vector.size()] = nullptr;
            this->setValues(strs, 0);
        }

        static ComboBox* CreateEx(const char* prefix, const char* suffix, std::vector<std::string> vector, bool direction, int i, bool trigger_callback, Callback callback) {
            ComboBox* obj = Create(prefix, suffix);
            obj->strValues(vector, direction);
            obj->set(i, trigger_callback);
            obj->setCallback(callback);
            return obj;
        }
    };

    class SelectionBox {
        typedef void(MH_CALL* Callback)(SelectionBox*);

       public:
        MH_DLL SelectionBox() = delete;
        MH_DLL SelectionBox(const SelectionBox&) = delete;
        MH_DLL SelectionBox& operator=(const SelectionBox&) = delete;
        ~SelectionBox() = delete;

        MH_DLL static SelectionBox* Create(const char* str);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void setValues(const char* strs[]);

        MH_DLL void set(int i, bool b);
        MH_DLL bool get(int i) const;

        void strValues(std::vector<std::string> vector, bool direction = MH_FWD) {
            const char* strs[MH_BOX_MAX];
            if(direction) {
                for(int i = vector.size() - 1; i >= 0; i--) {
                    strs[vector.size() - 1 - i] = vector[i].c_str();
                }
            } else {
                for(int i = 0; i < vector.size(); i++) {
                    strs[i] = vector[i].c_str();
                }
            }
            strs[vector.size()] = nullptr;
            this->setValues(strs);
        }

        static SelectionBox* CreateEx(const char* str, std::vector<std::string> vector, bool direction, int i, bool trigger_callback, Callback callback) {
            SelectionBox* obj = Create(str);
            obj->strValues(vector, direction);
            obj->set(i, trigger_callback);
            obj->setCallback(callback);
            return obj;
        }
    };

    class TextBox {
        typedef void(MH_CALL* Callback)(TextBox*, const char*);

       public:
        MH_DLL TextBox() = delete;
        MH_DLL TextBox(const TextBox&) = delete;
        MH_DLL TextBox& operator=(const TextBox&) = delete;
        MH_DLL ~TextBox() = delete;

        MH_DLL static TextBox* Create(const char* placeholder);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void set(const char* str);
        MH_DLL const char* get() const;

        void refresh() {
            this->set(this->get());
        }

        static TextBox* CreateEx(const char* placeholder, const char* set, Callback callback) {
            TextBox* obj = Create(placeholder);
            obj->setCallback(callback);
            if(set != nullptr) {
                obj->set(set);
            }
        }
    };

    class Spinner {
        typedef void(MH_CALL* Callback)(Spinner*, double value);

       public:
        MH_DLL Spinner() = delete;
        MH_DLL Spinner(const Spinner&) = delete;
        MH_DLL Spinner& operator=(const Spinner&) = delete;
        MH_DLL ~Spinner() = delete;

        MH_DLL static Spinner* Create(const char* prefix, const char* suffix);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void set(double v, bool trigger_callback = true);
        MH_DLL double get() const;

        void refresh(bool trigger_callback = true) {
            this->set(this->get(), trigger_callback);
        }

        static Spinner* CreateEx(const char* prefix, const char* suffix, double v, bool trigger_callback, Callback callback) {
            Spinner* obj = Create(prefix, suffix);
            obj->set(v);
            obj->setCallback(callback);
            return obj;
        }
    };

    class ColourPicker {
        typedef void(MH_CALL* Callback)(ColourPicker*, Colour);

       public:
        MH_DLL ColourPicker() = delete;
        MH_DLL ColourPicker(const ColourPicker&) = delete;
        MH_DLL ColourPicker& operator=(const ColourPicker&) = delete;
        MH_DLL ~ColourPicker() = delete;

        MH_DLL static ColourPicker* Create(Colour colour);

        MH_DLL void setCallback(Callback callback);

        MH_DLL void set(Colour colour, bool trigger_callback = true);
        MH_DLL Colour get() const;

        void refresh(bool trigger_callback = true) {
            this->set(this->get(), trigger_callback);
        }

        std::string getHexString() {
            return this->get().toHexString();
        }

        void setHexString(std::string HexColor) {
            if(HexColor.length() != 6) {
                HexColor = "000000";
            }

            Colour Color;
            Color.fromHexString(HexColor);

            this->set(Color);
        }

        static ColourPicker* CreateEx(Colour colour, Colour set, Callback callback) {
            ColourPicker* obj = Create(colour);
            obj->set(set);
            obj->setCallback(callback);
            return obj;
        }
    };
}