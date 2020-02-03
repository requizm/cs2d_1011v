# Counter Strike 2D Hack



<b>How To Use</b> <br/>
 <p><strong>How To Use</strong><br />Smart AimBot&nbsp;ON/OFF&nbsp; &nbsp; &nbsp;:&nbsp;&nbsp;&nbsp; [F5]<br />Esp ON/OFF&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; :&nbsp;&nbsp;&nbsp; [F6]<br />Anti&nbsp;Flash&nbsp;ON/OFF&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;:&nbsp;&nbsp;&nbsp; [F7]<br />No&nbsp;Scope&nbsp;Vision&nbsp;ON/OFF&nbsp; :&nbsp; [F8]</p>
   <br/> <br/>
   
 

<b>Hardly Recommended Settings</b><br/>
*Resolution: 1024x768, 800x600, 640x480<br/>
*Windowed<br/>
*4/3 Ratio: ON<br/>
*Graphic Driver: OpenGL<br/>
<br/> <br/><br/> <br/>

**What is  smart aimbot?**<br/>
It checks whether there is an object (wall etc.) between me and the opponent in each frame. If it does, aimbot doesn't work. If not, it works.

<br/> <br/><br/> <br/>
**Notlar**<br/>
Bu sefer, 1008v sürümündeki projeme nazaran daha stabil bir çalışma yapmış oldum. Bir önceki projemde çalışırken, entity list'i bulamamıştım. O yüzden bir takım hook'lar ile rakip base adresi alıyordum, bu sebepten dolayı bazen oyun crash veriyordu. Can değerini ise hiç bulamamıştım.  <br/>
Şimdikinde ise, SwapBuffer harici herhangi bir hook yok. Yani crash verme olasılığı çok düşük. Şarjör değiştirme, hareket, hasar verme vs bir takım fonksiyonları buldum, bunlar ne işe mi yarayacak? İstemci bazlı tabi, birkaç tanesi harici sunucularda çalışmayacak ama keyfine buldum işte. Can değerini çok güzel şifrelemişler. Güzel uğraştırdı ama değdi.<br/>
<br/><br/>
**Notes**<br/>
This time, I have done a more stable work than my project in the 1008v. While working on my previous project, I couldn't find the entity list. So I was getting a enemy base address with some hooks, so sometimes the game crashed. I had never found the address of health. Probably encrypted.<br/>
Currently, there is no hook other than SwapBuffer. So the probability of crash is very low. I found some functions like reload_ammo, move, damage, etc., what will they do? It is client based, of course, it will not work on servers except a few of them, but I have enjoyed it. They encrypted their health value very well. Finally i found it. 


