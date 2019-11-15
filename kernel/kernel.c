void main() {
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'B';
    video_memory+= 2;
    *video_memory = 'u';
}
