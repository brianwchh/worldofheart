****
- [查看更多文章請點擊此跳轉到博客目錄頁面](../../tableOfContent.md) 

****
#### 推薦文章

##### *_這篇文章比我在這裏分享的任何代碼和創業項目都重要，其中的發現關係到每一個人的方方面面。哲學比科學和技術更重要！哲學是人生，科學和技術只是喫飯而已！_*

#### 心智是可以被操控的！心智是可以被操控的！心智是可以被操控的！你所不知道的5G/6G微波腦機接口技術！ 

#### 點擊下面鏈接訪問此文章
- [https://github.com/brianwchh/worldofheart](https://github.com/brianwchh/worldofheart)

****

用不嚴謹的話概括：trait object就類似與其他語言中的接口(interface)，比如Golang。如果你沒有學過GoLang語言，也沒關係，我在這裏嘗試站在語言設計者的角度來介紹

   1. 設計接口interface的動機。
   2. 實現原理
   3. 注意事項   
 ---   

參考文檔: rust-programming-language-steve-klabnik, page 407,  Using Trait Objects That Allow for Values of Different Types  

因爲個人覺得這本好像是官方推薦的書本關於這塊寫的不容易理解，所以希望寫一篇博客，從自己理解的角度來介紹rust的trait object。希望能幫助初學者形象理解這方面的內容。


1. ### 設計接口interface的動機

   - 假設讀者已經理解了stack（棧）和heap（堆）的硬件知識   

      這個是理解Rust語言的關鍵，因爲rust語言的發明動機之一，就是如何在編譯之時防止stack and heap overflow（堆棧內存溢出），其優於C/C++之處就是因爲C/C++項目很大，或團隊成員之間合作時，往往會忘記手動刪除在heap上申請的內存，變成了垃圾碎片，導致電腦或嵌入式系統出現heap內存溢出，Rust的變量所有權機制很聰明地在編譯階段就幫我們排除了這樣的錯誤出現，相較於其他語言，比如有垃圾回收進程的GoLang等等，優點就是不需要額外的進程消耗資源來檢測和回收垃圾內存。這裏只簡單提及和複習下Rust語言發明的動機之一。

   - 發明rust的另一個主要動機 （非必要閱讀小節）
   
      當然其發明的另一個動機就是，相較於C/C++,在保證性能和其差不多時，又能實現非常方便部署和遷移，可以理解成生產的自動化，不需要像C/C++那樣，手動一個一個去下載依賴包，CMake File雖然比Makefile方便許多，但還是沒法做到一鍵自動化實現生產的環境重現，嘗試過opencv編譯的讀者應該有親身體會。爲了實現像javascript那樣實現nodemoudle的一鍵自動下載和安裝，Rust用Cargo來管理依賴的包，順便提及下Golang的包管理系統是go module，有興趣的可以看下我github上的pdf介紹文章：[https://github.com/brianwchh/web3-full-stack-design-tutorial/tree/main/05-go%20module/slice](https://github.com/brianwchh/web3-full-stack-design-tutorial/tree/main/05-go%20module/slice)。這裏也不展開，有興趣可以之後去瞭解，和對比這兩種語言的包管理方法，原理上都差不多。不理解這個包管理系統，不方案理解接口(interface)，這裏只是順帶提及下rust語言發明的另一個動機。

   - 必要前提，瞭解vector相關知識
      
      除了必要的stack和heap知識，希望讀者也瞭解了vector. 這裏簡要回顧下。

         let v: Vec<i32> = Vec::new();
         //  Creating a new, empty vector to hold values of type i32，

      簡單瞭解下在rust如何定義一個i32，即32bit的integer類型的vector v。  

      這裏請注意下，我們在用高級語言時，往往把它當成黑匣子，很多人也不去深究這一條短短的語句後面編譯器幫我們做了多少事情！如果不去深究和瞭解，即，站在底層電腦/嵌入式處理器的角度去看自己寫的代碼，你可能不知道自己寫的代碼，在compile之後會變成怎麼樣。  

      很多人也可能都沒寫過C語言，人們發明rust這些高級語言，有一個動機當然是希望程序員能少寫很多代碼，有些代碼可以通過compile去生成，但不代表我們不要去稍微瞭解下，其背後做了那些工作。

      比如，vector，這是一個大小可變型的內存，可以比較大，真正的數據放在stack肯定是不可能的，stack上只能放指針和描述型數據。這裏用下面一張圖來輔佐理解。

      <!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
      <div align="center" style="page-break-inside: avoid;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->

      <div style="display: flex; flex-direction: row; margin-top: 0px; margin-bottom: 0px;">

      <div style="flex-basics: auto;flex:1;"></div>



      <image style=" flex:0; width: 100%;  height:auto; -moz-opacity: 0.95; -khtml-opacity: 0.95; opacity: 0.99;" src='./images/Box_tutorial.png'/>


      <div style="flex-basics: auto;flex:1;"></div>

      </div>

      </div> <!-- end pictureWrapper_div -->

   暫時先不要去深究Box\<Type>,其實也不難，以後會介紹，簡單理解就是獲得一個heap上數據的指針，尤其是當這數據大小在compile階段無法知道時，用Box，其他的可以用其他例如reference和move等方式，這裏不深入介紹Box。放注意力放在我們如何處理在heap上的大數據內存，尤其是像vector這種大小在compile階段無法知道的，即在程序運行階段動態可變的。

   在C語言中，或者是彙編assemblly語言中，我們用鏈表的方式來實現vector，其實rust compiler也是這麼幫我們我們自動實現的，在C語言中，自己寫鏈表成員的添加和刪除還是蠻麻煩的，所謂鏈表，就是在heap的一段一段的小內存（vector成員在heap上的數據空間），裏面有指向上一個成員和下一個成員的地址，這樣，就像鏈表一樣，把vector內的各個成員連在一塊了，在查找的時候，就能循着這個鏈條尋邊所有成員。所有注意上圖heap上綠色和紅色部分的內存是不一定連續的，因爲，每次加入一個vector成員時，我們從高級語言用戶的角度，只是一小段語句，就像大老闆動動嘴，如下

         v.push(6);

   comopiler,卻要像C底層程序員那樣，去幫你生成往heap上申請內容的函數，比如c語言的是類似於int* faddr = malloc()，然後返回一個heap上的地址，如果某些原因申請不成功，faddr是負數，我們只要判斷下是否小於0，就知道是否成功了，這就是C語言程序的做法，但在寫程序的時候，我們常常忘記去判斷是否小於0，是人都會犯下這種很難查的失誤，所有常常會出現程序奔潰的很難查的bug。而rust編譯器幫我們生成的底層代碼，都是模塊化的，安全的代碼。

         vector<class> grade_ptr

   注意上面的vecotor里裝的成員class是指針，heap綠色部分，而每個指針指向heap上紅色的raw data。這就是大概數據的存放情況（物理，甚至虛擬內存都可能不連續）。








