# San-Juan
## About San Juan
* https://en.wikipedia.org/wiki/San_Juan_(card_game)
## Function
* 程式碼由上至下的函式分別為：
	* safe_flush：清除scanf的緩存
	* yourans：使用者輸入字串轉數字
	* setCardNum：亂數模擬抽牌
	* com_level2 : 在建築階段，當選擇Com是level 2 時會呼叫此函式
	* builder：建築階段
	* producer：生產階段
	* trader：商人階段
	* prospector：掏金者階段
	* counciler：市長階段
	* printallcom：列出３個Com的建築
	* printplayer：列出使用者的建築和牌
	* finalvps：遊戲結束後計算並顯示加上特殊牌的得分、顯示冠軍
	* main：選擇Com level，亂數抽governor和Com亂數選行動卡（例如：建築師）
## Variable
* 幾乎是全域變數
* 所有可以找出關連的變數都是用array連接
	* 有關牌的array有6個，分別為名稱、價格、得分、使用的階段（e.g. builder phase）以及每種牌的個數（110張牌分為29類）。
	* 與4個角色（包含使用者）有關的array有8個，除了名稱、分數、得到的牌總數、建築數、貨物數，還有用二維陣列表示手上有哪些牌、有哪些建築、哪些建築有生產貨物。
## main
* 初始化遊戲 : 每人都先有一張Indigo plant
* 將空的array的值從0改設為30，因為0是Smithy ( 特殊建築卡 )
* 呼叫setCardNum( )讓每位玩家一開始有4張牌
* 選擇Com的Level
* 印出遊戲畫面
	* 在印出遊戲畫面後都要按enter確定使用者知道現在進行到哪一步。
* 抽選governor，Com亂數選而使用者輸入要進行的階段
	* 每個階段輪四個人後換下一個governor，順序是 : Player → Com1 → Com2 → Com3 再回到Player的迴圈，角色選到的每個階段都會呼叫外面的函式處理。
* 判斷是否有玩家蓋12棟建築物，有就呼叫finalvps( )並結束遊戲
* 印出一回合後的遊戲畫面
* 判斷每一回合 ( 同一個governor ，四階段都完成 ) 結束後是否要使用特殊卡 ( Tower, Chapel )
## setCardNum
* 亂數抽牌
	* 亂數除以牌的總數再從第一種牌的個數開始減，減到等於零時就表示抽到那張牌，並將該種牌的個數減一，牌的總數減一。
* 沒有牌時
	* 將被丟棄的牌 ( 以discard[110] 儲存，一張張加回piece[ ] 對應種類的牌，再將被丟棄的牌總數 ( discard_sum ) 加到牌的總數 ( sum )中。
## com_level2
* 在建築師階段中，判斷Com的牌是否有可以生產或最後結束時可加分的牌，沒有就回到一般的建築方式 ( 見 builder )。
## builder
* tobuild表示要蓋player ( p ) 的第幾張牌
* 判斷如果p 是governor，可減一張牌的費用
* 如果p手上牌的總數為零，表示不能蓋建築，直接跳出
* 判斷是否有Black market的建築而且有生產貨物
	* 如果有，使用者可以選擇是否要用貨物抵費用，Com會用最多兩個貨物來抵費用。
* 判斷是否有Crane的建築
	* 如果有，使用者可選擇是否要用Crane的功能、用了之後要覆蓋在哪張建築物卡以及可抵多少費用；Com則是會隨機選一個建物覆蓋上去。
* 選擇要蓋哪張卡
	* 使用者在選擇後才會判斷可用哪種建築的特殊功能抵費用，所以可能會出現選擇後輸出input error的狀況，如果每張卡都不行蓋可選0退出。level 1的Com會從最後一張卡片開始判斷能否蓋，不行就在往前找直到小於0；level 2的Com會傳至函式com_level2，都沒有再回到level 1。
* 建造特殊建築且有Carpenter建築可加1手牌
* 將卡牌建造成建築
	* 如果有使用Crane且成功選到能變建築的卡，就會將原本的建築丟棄，減去原本建築所獲得的分數，如果有貨物也會消失，費用會變成減掉原本建築的費用；沒有Crane則是建在最後，增加建築數。兩者都會加得分，將卡從手牌刪除。
* 用手牌付費
	* 如果費用小於等於0，不需要支付手牌，判斷是否有Poor house且符合條件就加手牌，然後結束。
	* 如果費用等於手牌，將手牌全部丟棄，手牌總數設為0。
	* 如果以上兩者都不符合，使用者可選擇要用哪些手牌支出，Com則是由後往前支出手牌。
* 判斷是否有Poor house且符合條件就加手牌
## producer
* 計算該玩家最多可以生產多少貨物
	* Governor、Aqueduct、Library
* 計算現在有多少生產建築 ( production )
* 計算可以生產多少建築
	* 可生產值 = prodution-product_sum[p]
		* 如果可生產值大於等於最大可生產值，會全部生產。
		* 如果可生產值大於最大可生產值，就需要選擇生產。
* 選擇生產
	* 使用者可選擇要在哪些尚未有貨物的生產建築生產，或選擇0不生產。
	* Com會從費用較高的生產建築開始生產。
* wantproduce表示實際生產數
	* 輸出最終該玩家生產多少貨物，如果大於1個會在判斷是否有Well建築，可以增加一張手牌。
## trader
* 計算該玩家最多可以販售多少貨物
	* Governor, Library, Trading post
* 販售貨物
	* 使用者可選擇要賣幾個貨物或不賣；Com會從建築費用較高的貨物開始販售，存取販售後能獲得的牌數 ( 金錢 )。
* 增加手牌的特殊建物卡
	* Market stand, Market hall
* 全部結束後再抽牌
## prospector
* 計算能抽幾張牌，並先抽牌
	* Governor, Library
* 玩家有Gold mine
	* 會亂數抽4張牌，如果有任2張是相同費用，要全數丟棄；如果費用都不相同，使用者可以選擇要哪張牌，Com會選擇第一張，並將剩下三張牌丟棄。
## councilor
* 計算能抽幾張牌( draw )
	* Governor, Library
* 計算能選幾張牌入手牌 ( getcard )
	* Prefecture
* 從抽到的牌中選擇要入手的牌
	* 使用者如果有Archive可以從原本的手牌加上抽取的牌中抽出( 手牌 + getchar )，沒有的話就是從抽到的牌開始抽選；Com無論是否有Archive都會從抽到的牌開始抽選。
## finalvps
* 計算最終得分，如果有特殊的牌會再加分
* 	Guild hall, City hall, Triumphal arch, Palace
* 印出所有人的得分和冠軍
