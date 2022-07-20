namespace XorShift {
typedef unsigned int uint;
uint seed = 1;
uint rnd() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}
} // namespace XorShift
