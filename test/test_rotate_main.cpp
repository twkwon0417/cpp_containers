// Standalone test runner for all map helper functions
#include "test_core.h"
#include "test_print.h"

void test_map_rbt_helpers();
void test_map_find();
void test_map_insert();
void test_map_erase();

int main() {
  print_module_banner("Map Helper Functions Tests");
  test_map_rotate();
  test_map_rbt_helpers();
  test_map_find();
  test_map_insert();
  test_map_erase();
  return 0;
}
