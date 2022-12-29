# mt_engine_20


Dependencies:
- truetype
- catch2

Converted to use modules and c++20

```
static const int COMPILE_TIME_CONSTANT;

auto& variable_name = 0;

Object& some_object_reference;
Object* some_object_pointer;

enum struct EnumName 
{
    ENUMERATION_ONE,
    ENUMERATION_TWO,
}

do
{
    // do stuff
} while (condition);

while (condition)
{

}

for (auto i = 0u; i < 0; i++) 
{
    // iterate over stuff
}

auto Lamda = [&](int param_1, int param_2) -> int 
{
    return 0;
}

void FreeFunction;

template<typename TYPE>
class ClassName 
{	
    int _private_member;

    void _privateFunction(int some_parameter_1, int some_parameter_2)

public:
    
    int public_member;

    void publicFunction(int some_parameter_1, int some_parameter_2);
}

```
