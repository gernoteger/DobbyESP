(function(){var c=" deaktivieren",d=".",e="?",h="Alles in folgende Sprache \u00fcbersetzen: ",k="Deaktivieren f\u00fcr: ",l="Diese Seite anzeigen auf: ",m="Diese Seite mit Google \u00dcbersetzer in folgende Sprache \u00fcbersetzen: ",n="Google hat diese Seite automatisch in folgende Sprache \u00fcbersetzt: ",p="Powered by ",q="var ",r="\u00dcbersetzt in: ",t="\u00dcbersetzung aus ",u=this;
function v(a,x){var f=a.split(d),b=u;f[0]in b||!b.execScript||b.execScript(q+f[0]);for(var g;f.length&&(g=f.shift());)f.length||void 0===x?b[g]?b=b[g]:b=b[g]={}:b[g]=x};var w={0:"\u00dcbersetzen",1:"Abbrechen",2:"Schlie\u00dfen",3:function(a){return n+(a+d)},4:function(a){return r+a},5:"Fehler: Der Server kann Ihre Anfrage momentan nicht verarbeiten. Bitte versuchen Sie es sp\u00e4ter erneut.",6:"Weitere Informationen",7:function(a){return p+a},8:"\u00dcbersetzer",9:"\u00dcbersetzung wird erstellt.",10:function(a){return m+(a+e)},11:function(a){return l+a},12:"Original anzeigen",13:"Der Content dieser lokal gespeicherten Datei wird \u00fcber eine sichere Verbindung zur \u00dcbersetzung an Google gesendet.",
14:"Der Content dieser sicheren Seite wird \u00fcber eine sichere Verbindung zur \u00dcbersetzung an Google gesendet.",15:"Der Content dieser Intranetseite wird \u00fcber eine sichere Verbindung zur \u00dcbersetzung an Google gesendet.",16:"Sprache ausw\u00e4hlen",17:function(a){return t+(a+c)},18:function(a){return k+a},19:"Immer ausblenden",20:"Originaltext:",21:"Bessere \u00dcbersetzung vorschlagen",22:"Vorschlagen",23:"Alles \u00fcbersetzen",24:"Alles wiederherstellen",25:"Alles abbrechen",26:"Abschnitte in meine Sprache \u00fcbersetzen",
27:function(a){return h+a},28:"Originalsprachen anzeigen",29:"Optionen",30:"\u00dcbersetzung f\u00fcr diese Website deaktivieren",31:null,32:"Alternative \u00dcbersetzungen anzeigen",33:"Klicken Sie auf die oben stehenden W\u00f6rter, um alternative \u00dcbersetzungen abzurufen.",34:"Verwenden",35:"Zum Verschieben mit Shift ziehen",36:"Zum Aufrufen alternativer \u00dcbersetzungen klicken",37:"Halten Sie Shift gedr\u00fcckt, klicken Sie und ziehen Sie dann die oben stehenden W\u00f6rter, um sie neu zu sortieren.",
38:"Wir danken f\u00fcr Ihren Vorschlag zu Google \u00dcbersetzer.",39:"\u00dcbersetzung f\u00fcr diese Website verwalten",40:"Klicken Sie auf ein Wort, um alternative \u00dcbersetzungen zu sehen, oder doppelklicken Sie, um es direkt zu bearbeiten.",41:"Originaltext",42:"\u00dcbersetzer",43:"\u00dcbersetzen",44:"Ihre Korrektur wurde gesendet.",45:"Fehler: Die Sprache der Webseite wird nicht unterst\u00fctzt."};var y=window.google&&google.translate&&google.translate._const;
if(y){var z;a:{for(var A=[],B=["26,0.01,20150908"],C=0;C<B.length;++C){var D=B[C].split(","),E=D[0];if(E){var F=Number(D[1]);if(!(!F||.1<F||0>F)){var G=Number(D[2]),H=new Date,I=1E4*H.getFullYear()+100*(H.getMonth()+1)+H.getDate();!G||G<I||A.push({version:E,ratio:F,a:G})}}}for(var J=0,K=window.location.href.match(/google\.translate\.element\.random=([\d\.]+)/),L=Number(K&&K[1])||Math.random(),C=0;C<A.length;++C){var M=A[C],J=J+M.ratio;if(1<=J)break;if(L<J){z=M.version;break a}}z="27"}var N="/translate_static/js/element/%s/element_main.js".replace("%s",
z);if("0"==z){var O=" translate_static js element %s element_main.js".split(" ");O[O.length-1]="main_de.js";N=O.join("/").replace("%s",z)}if(y._cjlc)y._cjlc(y._pas+y._pah+N);else{var P=y._pas+y._pah+N,Q=document.createElement("script");Q.type="text/javascript";Q.charset="UTF-8";Q.src=P;var R=document.getElementsByTagName("head")[0];R||(R=document.body.parentNode.appendChild(document.createElement("head")));R.appendChild(Q)}v("google.translate.m",w);v("google.translate.v",z)};})()
