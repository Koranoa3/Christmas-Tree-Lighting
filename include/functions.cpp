
template<typename T>
T smoothstep(T edge0, T edge1, T x) {
    T t;
    t = clamp((x - edge0) / (edge1 - edge0), static_cast<T>(0.0), static_cast<T>(1.0));
    return t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t);
}

template<typename T>
T clamp(T value, T min_val, T max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}