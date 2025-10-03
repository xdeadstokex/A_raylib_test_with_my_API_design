script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd $script_dir

src_dir=""
build_dir=$script_dir/build

raylib_include_dir=$script_dir/lib/raylib-5.5_linux_amd64/include/
raylib_lib_dir=$script_dir/lib/raylib-5.5_linux_amd64/lib/

gcc -c module_app_core_raylib.c -o $build_dir/core.o -I$raylib_include_dir -I$script_dir -I./lib
gcc -c core_raylib_test_module.c -o $build_dir/main.o -I$script_dir -I./lib
gcc -o main $build_dir/main.o $build_dir/core.o -L$raylib_lib_dir -l:libraylib.a -lm

rm -f $build_dir/main.o $build_dir/core.o
