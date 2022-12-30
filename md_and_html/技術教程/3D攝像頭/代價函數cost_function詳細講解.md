****
- [<font size=3>跳轉到博客目錄頁面</font>](../../../tableOfContent.md)[<font size=2>在線閱讀</font>]&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;  <font size=2> [本地] </font><font size=3>[*_點擊此查看html網頁格式_*](../../../tableOfContent.html)&nbsp; &nbsp; [*_pdf格式_*](../../../tableOfContent.md.pdf)</font>
****

##### *_這篇文章比我在這裏分享的任何代碼和創業項目都重要，其中的發現關係到每一個人的方方面面。哲學比科學和技術更重要！哲學是人生，科學和技術只是喫飯而已！_*

#### 心智是可以被操控的！心智是可以被操控的！心智是可以被操控的！你所不知道的5G/6G微波腦機接口技術！ 

#### 點擊下面鏈接訪問
- [<font color=red>無眠月照無情門 . 失去自由的歌手</font>](https://github.com/brianwchh/worldofheart/blob/main/md_and_html/%E7%84%A1%E7%9C%A0%E6%9C%88%E7%85%A7%E7%84%A1%E6%83%85%E9%96%80.md)<font size=1> [點擊此前往github在線閱讀]</font> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <font size=1>本地模式 &nbsp;[html網頁版](../../../md_and_html/無眠月照無情門.html) &nbsp;&nbsp;&nbsp; [pdf版本](../../../md_and_html/無眠月照無情門.md.pdf) </font>
- 心学新解 : [https://github.com/brianwchh/worldofheart](https://github.com/brianwchh/worldofheart)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <font size=1>本地模式 &nbsp;[html網頁版](../../../md_and_html/心學新解.html) &nbsp;&nbsp;&nbsp; [pdf版本](../../../md_and_html/心學新解.md.pdf) </font>

****

****<p align="center" style="font-size: 28px;">代價函數cost_function詳細講解</p>****

<p align="center" style="font-size: small;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 阿柄</p>


</br>


本次算法是一個改進版本的SGM，參考論文見如下圖片中所列出的。不是必要求去閱讀，這篇教程應該會比那個論文更容易理解。

<!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
<div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
  <div class="ImageWrapperFlex" >
   <div class="FlexSide"  ></div>
   <image class="FlexImage"   src='./images/代價函數cost_function詳細講解1.png'/>
   <div class="FlexSide" ></div>
  </div>
  <p align="center" style="margin:0px;">   </p> 
</div> <!-- end pictureWrapper_div -->

首先不要被上面的公式嚇到了，理解其物理意義就不難了。

- 在解釋特徵值代價函數之前，我們先回顧下幾個必要的知識： 

    - [Disparity 像素視差](#Disparity)
    - [Census transform 比中二值編碼](#比中二值編碼) 
    - [matching cost function 特徵值匹配函數](#特徵值匹配函數)



#### <p id="Disparity"> <p/>
* #### Disparity 像素視差
<!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
<div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
  <div class="ImageWrapperFlex" >
   <div class="FlexSide"  ></div>
   <image class="FlexImage"   src='./images/代價函數cost_function詳細講解2.png'/>
   <div class="FlexSide" ></div>
  </div>
  <p align="center" style="margin:0px;">   </p> 
</div> <!-- end pictureWrapper_div -->

如上所示，同一個點P在雙目左右兩個攝像頭中的成像像素位置不一樣，在相機1中的水平像素值爲<span style="font-size: 30px;">x</span><span style="font-size: 10px;">l</span>和<span style="font-size: 30px;">x</span><span style="font-size: 10px;">r</span>,因此，P(x,y,z)的3D位置信息就包含在Disparity的值D中。

**_雙目攝像頭的目的就是找到每個像素的Disparity值，由此就能算出3D信息(x,y,z)。而計算Disparity值，前提是要找到左右兩圖中各個像素的匹配點，即，左圖的每個像素對應右圖中的哪個像素？我們肉眼是能找到，但程序需要如何匹配呢，找到了匹配點，就能算出Disparity了。_**  

**_因而，真正的難點是如何找左右兩圖的每個像素的匹配點。_**



</br>
</br>


<p align="right"> 2022年12月31日 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </p>


</br>
</br>

<style>

.ImageWrapperFlex {
    display: flex; 
    flex-direction: row; 
    margin-top: 1px; 
    margin-bottom: 1px;

    width: 100% ;
}

.FlexSide {
    flex-basis: 0px ;
    flex:1;

}



/* large device screen 設置熒幕顯示圖片大小（電腦等大型屏幕）*/
@media only screen and (min-width: 600px) {

    .FlexImage {
        flex-basis: 900px ;
        flex:0;    
        height:auto; 
        max-width: 900px;
        min-width: 900px;
     
    }

}

 /* small device screen 設置熒幕顯示圖片大小（平板手機等屏幕）*/
@media only screen and (max-width: 600px) {
    
    .FlexImage {
        flex-basis: 600px ;
        flex:1;
        height:auto; 
     
    }

}

/* style for print !important 設置打印圖片大小*/
@media print {

    .FlexImage {
        flex-basis: 600px ;
        flex:0;    
        height:auto; 
        max-width: 600px;
        min-width: 600px;
     
    }
}

</style>


<!-- 共用的css -->
<!-- <head>
    <link rel="stylesheet" href="../common_css/common_style.css">
</head> -->



