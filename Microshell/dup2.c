// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>

// int main() {
//     int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     int tmp_fd = STDOUT_FILENO;
// 	dup2(fd, STDOUT_FILENO); //ÇIKTI yönü fd oluyor.
//     close(fd);
// 	// write(0, "asdasda\n", 9);
// 	dup2(tmp_fd, STDOUT_FILENO);
//     dup2(STDOUT_FILENO, STDIN_FILENO);
//     printf("Bu metin output.txt dosyasına yazdırılacak.\n");
    
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int saved_stdout = dup(STDOUT_FILENO); // önceden tutulmuş kopyayı oluştur
    
    dup2(fd, STDOUT_FILENO); // STDOUT'u output.txt dosyasına yönlendir
    
    printf("Bu metin output.txt dosyasına yazdırılacak.\n");
    
    dup2(saved_stdout, STDOUT_FILENO); // STDOUT'u eski haline geri getir
    
    printf("Bu metin tekrar standart çıkışa yazdırılacak.\n");
    
    close(fd);
    return 0;
}

// int main() {
//     int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     int saved_stdout = dup(STDOUT_FILENO); // önceden tutulmuş kopyayı oluştur
    
//     dup2(fd, STDOUT_FILENO); // STDOUT'u output.txt dosyasına yönlendir
    
//     printf("Bu metin output.txt dosyasına yazdırılacak.\n");
    
//     fflush(stdout); // stdout bufferını temizle
    
//     dup2(saved_stdout, STDOUT_FILENO); // STDOUT'u eski haline geri getir
    
//     printf("Bu metin tekrar standart çıkışa yazdırılacak.\n");
    
//     close(fd);
//     return 0;
// }
