/* github.com/rubicks/libmorton/test/iface.c */

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "binlit.h"
#include "morton.h"

/* returns the largest power of two that does not exceed the given number */
static unsigned long
_floor2(unsigned long v)
{
  unsigned long ret;
  do {
    ret = v;
  } while (v &= v - 1);
  return ret;
}

static unsigned long
_cardinality(unsigned long v)
{
  unsigned long c = 0;
  for (; v; ++c) {
    v &= v - 1;
  }
  return c;
}

static unsigned
_part3(unsigned v)
{
  v &= 0xffffff;
  v = (v | (v << 16)) & (unsigned)0x0000ff;
  v = (v | (v << 8)) & (unsigned)0x00f00f;
  v = (v | (v << 4)) & (unsigned)0x0c30c3;
  v = (v | (v << 2)) & (unsigned)0x249249;
  return v;
}

int
main(int argc, char** argv)
{
  for (int i = 0; i < argc; ++i) {
    printf("argv[%d] == \"%s\"\n", i, argv[i]);
  }

  assert(32 == _floor2(42));

  assert(B8(1) == 1);
  assert(B8(10) == 2);
  assert(B8(100) == 4);
  assert(B8(1000) == 8);
  assert(B8(10000) == 16);
  assert(B8(100000) == 32);
  assert(B8(1000000) == 64);

  assert(3 == _cardinality(42));
  assert(3 == _cardinality(41));
  assert(3 == _cardinality(37));
  assert(3 == _cardinality(35));

  for (unsigned v = 0; v < 256; ++v) {
    assert(morton_encode(v << 0 * CHAR_BIT) == morton_expand(v) << 0);
    assert(morton_encode(v << 1 * CHAR_BIT) == morton_expand(v) << 1);
    assert(morton_encode(v << 2 * CHAR_BIT) == morton_expand(v) << 2);
  }

  assert(0x249249 == _part3(0xff));

  for (unsigned v = 0; v < 0x1000000; ++v) {
    assert(morton_decode(morton_encode(v)) == v);
  }

  return EXIT_SUCCESS;
}
