![Demo](https://private-user-images.githubusercontent.com/72830904/584999910-d2e0fe29-7141-478a-8a4c-4b934c9a3319.gif?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3Nzc0MDEwNDYsIm5iZiI6MTc3NzQwMDc0NiwicGF0aCI6Ii83MjgzMDkwNC81ODQ5OTk5MTAtZDJlMGZlMjktNzE0MS00NzhhLThhNGMtNGI5MzRjOWEzMzE5LmdpZj9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNjA0MjglMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjYwNDI4VDE4MjU0NlomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTY0OGQyZTYwN2QzYjdjNmI4ZDBlNjVjZWQ5ODBmY2NhYWU3NzYyODkxMjU1ZTRkMzIzNzU4ZWJlOGRmNTU5MGImWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JnJlc3BvbnNlLWNvbnRlbnQtdHlwZT1pbWFnZSUyRmdpZiJ9.esnPtA07kkNK4_aMFVhdI8ca7PljW09vVN7dlBFPVLg)
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
