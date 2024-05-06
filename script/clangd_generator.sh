cat << EOF > .clangd
CompileFlags:
  Add:  [
    -Wall,
    -Wextra,
    -I$(realpath googletest/googletest/include),
    -I$(realpath googletest/googlemock/include),
    -I$(realpath header),
    -I$(realpath src),
  ]
EOF
echo Clangd correctly configured
