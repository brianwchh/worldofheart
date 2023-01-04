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


本次算法是一個改進版本的SGM，參考論文見如下圖片中所列出的。不是必要求去閱讀，這篇教程應該會比那個論文更容易理解。也希望能做出最深入淺出的一個雙目sgm講解版本。

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
        - [Census transform C++程序實現](#Census_transform程序實現)
    - [matching cost function 特徵值匹配函數](#特徵值匹配函數)



<a id="Disparity"> </a>

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

    **_因而，真正的難點是如何找左右兩圖的每個像素的匹配點。_** 而且這就是所有雙目算法要解決的核心問題。說起來很簡單，即在左右圖像校準好之後，所謂校準好就是說保證所有的匹配點在同一水平線上，即<span style="font-size: 40px;">X</span><span style="font-size: 20px;">l</span><span style="font-size: 40px;">(</span><span style="font-size: 23px;">x<span style="font-size: 17px;">l</span></span>,<span style="font-size: 23px;">y<span style="font-size: 17px;">l</span></span><span style="font-size: 40px;">)</span> 和  <span style="font-size: 40px;">X</span><span style="font-size: 20px;">r</span><span style="font-size: 40px;">(</span><span style="font-size: 23px;">x<span style="font-size: 17px;">r</span></span>,<span style="font-size: 23px;">y<span style="font-size: 17px;">r</span></span><span style="font-size: 40px;">)</span> 中 <span style="font-size: 23px;">y<span style="font-size: 17px;">l</span></span> = <span style="font-size: 23px;">y<span style="font-size: 17px;">r</span></span>，但算法做起來卻很複雜，我們的大腦能很輕鬆地辨認兩張圖像中相對應的匹配點，用程序來做卻很複雜，我們無法確切知道大腦是如何做到的，我們只能站在我們自己理解的角度去實現，有可能我們的大腦根本不是這麼實現的。  

    <!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
    <div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
    <div class="ImageWrapperFlex" >
    <div class="FlexSide"  ></div>
    <image class="FlexImage"   src='./images/代價函數cost_function詳細講解3.png'/>
    <div class="FlexSide" ></div>
    </div>
    <p align="center" style="margin:0px;">   </p> 
    </div> <!-- end pictureWrapper_div -->

    以上就是disparity及其用途和計算公式。 
    
    ***_爲了不造成更多的理解困難，這裏只列出其distance（深度信息）即 z 如何從disparity和像素座標(r,c)信息中提取3D座標的x,y值會在以後的教程中提及，需要更多的機器視覺(computer vision)的內容作鋪墊，一步一步來，不急。_*** 


<a id="比中二值編碼"> </a>

* #### census transform 比中二值編碼

    <!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
    <div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
    <div class="ImageWrapperFlex" >
    <div class="FlexSide"  ></div>
    <image class="FlexImage"   src='./images/代價函數cost_function詳細講解4.png'/>
    <div class="FlexSide" ></div>
    </div>
    <p align="center" style="margin:0px;">   </p> 
    </div> <!-- end pictureWrapper_div --> 

    <!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
    <div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
    <div class="ImageWrapperFlex" >
    <div class="FlexSide"  ></div>
    <image class="FlexImage"   src='./images/代價函數cost_function詳細講解5.png'/>
    <div class="FlexSide" ></div>
    </div>
    <p align="center" style="margin:0px;">   </p> 
    </div> <!-- end pictureWrapper_div --> 

    所謂census transform，我記不清中文翻譯是什麼名字了，這裏且稱之爲比中二值編碼。爲何要這個census transform呢？ 

    原因如果只是顏色，比如我們常用的grayscale，不足以做左右兩邊像素點的匹配，我們不能說兩邊圖像的像素值一樣就是匹配點，這是不可能做好的，首先，由於左右圖像分別來自兩個攝像頭，其像素值是一般很難相同的，因爲曝光參數很難做到兩邊一致。

    那有什麼特徵能用來描述兩張照片中的匹配點呢？很自然地，我們會想到computer vision知識里的各種特徵算子，比如surf之類的（逃難3年度多沒碰這內容了，交還給老師了，反正呢這時ROS機器人slam里常用的，屬於比原始的scale shift rotation invariant計算起來比較快的，其核心就是計算像素點其各個方向的梯度，然後以其最大值爲index0，然後逆時針方向做成一個一維vector，用來表徵其與相鄰像素的顏色變化關係，這樣就比單純用像素值來做匹配靠譜多了。），但這種特徵點匹配算法過於複雜，而且其包含的周圍像素也就那麼幾個，即上下左右，然後就是斜對角的幾個鄰居。這種算子計算量大。

    如何找一個計算速度快，而且又靠譜的算法，用以表徵某像素與鄰居的顏色漸變關係呢？ census transform提出了一個簡潔的做法，即以此像素爲中心，畫一個方框，比如5x7,長7個像素，寬5個像素，然後框內的鄰居的像素值與該中心像素做比較，比之大爲1，反之爲0，這樣就能形成一個二值化的特徵map，然後按照固定順序將它展開，形成一個一維向量，這樣的census transform之後的左右圖像信息來做匹配。這種特徵算法沒考慮scale，和rotation，但卻依然能很有效地工作，是因爲我們雙目本身依據校準過了，沒有rotation，其匹配目標都在一條水平線上，scale也一致。如上圖。    


    * #### <a id="Census_transform程序實現"> </a> <a name="Census_transform程序實現"> </a>Census transform C++ 程序實現




### <a id="特徵值匹配函數"> </a>

* #### matching cost function 特徵值匹配函數


    <!-- image area, flex to make it center,it may not work for github, for html and pdf rendering only -->
    <div align="center" style="page-break-inside: avoid; margin-top:1px; margin-bottom:1px;"> <!-- pictureWrapper_div add this only to make the bendan github understand -->
    <div class="ImageWrapperFlex" >
    <div class="FlexSide"  ></div>
    <image class="FlexImage"   src='./images/代價函數cost_function詳細講解5.png'/>
    <div class="FlexSide" ></div>
    </div>
    <p align="center" style="margin:0px;">   </p> 
    </div> <!-- end pictureWrapper_div -->    

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



