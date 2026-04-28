![hippo](https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExendoMzBxOWpuNnI5OThjdjk4M3FhcjVyYTQzNHM3dHF3ZjU4eTJuaCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/WBpdreVQWpd8JAkxsm/giphy.gif)
- found entity list
- reversed the entity structure
- i've found the projection matrix
- i think i found the camera rotation matrix but the translation bit isn't there to make a whole view matrix.
- i_hat = right
j_hat = forward
k_hat = up

~~game view matrix i think
0.991 0.063 -0.119 27
0.119 0.008 0.993 95
-0.064 0.998 0 12
0 0 0 1~~

mine
-0.119 0.993 0 -9.068
0.063 0.008 0.998 -97.411
-0.991 -0.119 0.064 23.101
0 0 0 1
