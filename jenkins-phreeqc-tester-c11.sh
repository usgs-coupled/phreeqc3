#!/bin/sh
failed=0
rel_error=1e-3
cd src || exit

./bootstrap     || exit
mkdir -p _build || exit
cd _build       || exit
../configure    || exit
make -j 20      || exit
cd ../mytest    || exit

test(){
  echo "Testing ${1}"
  mv "${1}_101.sel" "${1}_101.sel.expected"
  make "${1}.out" > /dev/null
  diff "${1}_101.sel" "${1}_101.sel.expected"  > "${1}.diff"
  numdiff --relative-tolerance"=${rel_error}" "${1}_101.sel" "${1}_101.sel.expected"  > "${1}.numdiff"
  if [ "$?" != "0" ]; then
    echo "  FAILED"
    head "${1}.diff"
    failed=1
  fi
}

for f in *_101.sel; do
  f=`echo "${f}" | sed 's/_101\.sel$//'`
  test "${f}"
done

if [ ${failed} != 0 ]; then
  false
fi
