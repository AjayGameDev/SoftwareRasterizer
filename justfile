run:
    cmake --preset linux 
    cmake --build build/linux 
    ./build/linux/main


push message="update":
    git add .
    git commit -m "{{message}}"
    git push origin main
    
