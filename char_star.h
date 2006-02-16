#if !defined(CHARSTAR_H_INCLUDED)
#define CHARSTAR_H_INCLUDED

struct CHARSTAR_LESS : std::binary_function<const char*, const char*, bool> {
bool operator()(const char* _X, const char* _Y) const
{
        return ::strcmp( _X, _Y) < 0;}
};

#endif

