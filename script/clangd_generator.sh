cat << EOF > .clangd
CompileFlags:
  Add:
    - "-Wall"
    - "-Wextra"
    - "-l$(realpath googletest/googletest/lib/libgtest.a)"
    - "-l$(realpath googletest/googletest/lib/libgmock.a)"
    - "-I$(realpath googletest/googletest/include)"
    - "-I$(realpath googletest/googlemock/include)"
    - "-I$(realpath header)"
    - "-I$(realpath src)"

---
If:
  PathMatch: .*_test\.cpp

Diagnostics:
  Suppress: access
EOF
echo Clangd correctly configured
