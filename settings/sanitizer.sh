
_gb_sanitizer_src="${BASH_SOURCE[0]:-$0}"
_gb_settings_dir="$(cd "$(dirname "${_gb_sanitizer_src}")" && pwd)"

case "$(uname -s)" in
Linux)
  export LD_PRELOAD="$(g++ -print-file-name=libasan.so):$(g++ -print-file-name=libstdc++.so.6)"
  export LSAN_OPTIONS="suppressions=${_gb_settings_dir}/../onboard/lsan.supp"
  ;;
Darwin)
  export DYLD_INSERT_LIBRARIES="$(clang -print-file-name=libclang_rt.asan_osx_dynamic.dylib)"
  ;;
*)
  echo "sanitizer.sh: unsupported platform $(uname -s)" >&2
  ;;
esac

unset _gb_sanitizer_src _gb_settings_dir
