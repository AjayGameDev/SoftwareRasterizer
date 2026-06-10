run:
    cmake --preset Linux 
    cmake --build build/Linux 
    ./build/Linux/main

release:
        cmake --preset Linux -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native"
        cmake --build build/Linux --config Release
        ./build/Linux/main


push message="update":
    git add .
    git commit -m "{{message}}"
    git push origin main
    
