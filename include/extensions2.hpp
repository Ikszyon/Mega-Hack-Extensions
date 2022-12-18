#pragma once
#define MH_DLL __declspec(dllimport)
#define MH_CALL __stdcall
#define MH_FWD 1
#define MH_BWD 0

#ifndef MH_BOX_MAX
#define MH_BOX_MAX 128
#endif

#include <string>
#include <vector>

namespace MegaHackExt {

    struct Colour {
        unsigned char r, g, b;
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
            for(auto i = elements.rbegin(); i != elements.rend(); i++) {
                this->add(*i);
            }
            if(auto_commit) {
                Client::commit(this);
            }
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

        static Button* WithCallback(const char* str, Callback callback) {
            Button* Btn = Create(str);
            Btn->setCallback(callback);
            return Btn;
        }
    };

    class MH_DLL CheckBox {
        typedef void(MH_CALL* Callback)(CheckBox*, bool);

       public:
        CheckBox() = delete;
        CheckBox(const CheckBox&) = delete;
        CheckBox& operator=(const CheckBox&) = delete;
        ~CheckBox() = delete;

        static CheckBox* Create(const char* str);

        void setCallback(Callback callback);

        void set(bool b, bool trigger_callback = true);
        bool get() const;
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

        void increment(int max_index, bool trigger_callback = true) {
            // get max_index by calling strValues, which returns an int
            // if you set max_index to a value that is too large gd will crash
            if(this->get() != max_index) {
                this->set(this->get() + 1, trigger_callback);
            }
        }

        void decrement(bool trigger_callback = true) {
            if(this->get() != 0) {
                this->set(this->get() - 1, trigger_callback);
            }
        }

        int strValues(std::vector<std::string> vector, bool direction = MH_FWD) {
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
            return vector.size() - 1;
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

        int strValues(std::vector<std::string> vector, bool direction) {
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
            return vector.size() - 1;
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

        static TextBox* CreateHex(const char* placeholder) {    // idc that this is bad
            TextBox* box = Create(placeholder);

            box->setCallback([](TextBox* temp, const char* str) {
                std::string text = str;
                text.erase(remove_if(text.begin(), text.end(), [](char current) {return !isxdigit(current);}), text.end());
                temp->set(text.c_str());
            });

            return box;
        }

        void refresh() {
            this->set(this->get());
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
    };
}