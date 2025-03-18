!! READ BEFORE STARTING APPLICATION !!

* <time.h> digunakan untuk logging saja (menyimpan tanggal dan waktu device)
* #define _CRT_SECURE_NO_WARNINGS digunakan karena vscode aku nunjukkin warning kalau scanf dan strcpy deprecated

* Sebelum prompt apapun, aplikasi membaca terlebih dahulu file 'product_database.txt'

Sebelum aplikasi jalan, user diminta memasukkan saldo awal 
!! pastikan input tidak negative dan bukan string (error)


Search Product Menu:
- Pada menu Search Product, semua list barang ditampilkan agar dapat melihat produk apa saja yang ada
  !! input bersifat case sensitive, pastikan searching sesuai dengan yang ada pada list produk
- Ketika produk ketemu, user ditanya apakah ingin membeli produk atau tidak, kalau iya maka diminta kuantitas
- Setelah membeli ditampilkan total akhir berdasarkan kuantitas yang dibeli

View Search History:
- Halaman ini menampilkan 5 search terbaru (termasuk produk yang tidak ditemui aplikasi)

View Buy History:
- Halaman ini menampilkan 5 buy history terbaru, termasuk harga per barang dan kuantitas yang dibeli

View Total Spent:
- Menampilkan berapa banyak uang yang digunakan untuk 'membeli' produk

Fitur Logging:
- Logging ke dalam file userlogbook.txt
- menggunakan <time.h>
- Aktivitas yang di-log aplikasi:
	- Buy (Ketika user membeli barang)
	- Search (Ketika user search barang)
	- VIEW_BUY_HISTORY (Ketika user membuka halaman buy history)
	- VIEW_TRANSACTION (Ketika user membuka halaman total spent)
	- VIEW_SEARCH_HISTORY (Ketika user membuka halaman search history)

Fitur Ekstra:
- clear screen setiap pindah halaman
- user saldo yang diupdate setiap transaksi (dilihat pada main menu page)


!! THANKS FOR READING !!

