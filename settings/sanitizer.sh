export LD_PRELOAD="$(g++ -print-file-name=libasan.so):$(g++ -print-file-name=libstdc++.so.6)"
export LSAN_OPTIONS=suppressions=$(dirname $0)/../onboard/lsan.supp