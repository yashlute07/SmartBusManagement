(function () {
    "use strict";

    let session = {};
    try {
        session = JSON.parse(localStorage.getItem("smartBusSession") || "null") || {};
    } catch (_) {
        session = {};
    }
    if (!session.role) return;

    const role = session.role;
    const name = session.name || (role === "admin" ? "Admin" : "there");
    let lastTopic = "general";
    let conversation = [];

    const style = document.createElement("link");
    style.rel = "stylesheet";
    style.href = "chatbot.css";
    document.head.appendChild(style);

    const launcher = document.createElement("button");
    launcher.id = "tripxChatLauncher";
    launcher.innerHTML = "&#128172;";
    launcher.title = "Chat with TripX Assistant";

    const panel = document.createElement("div");
    panel.id = "tripxChatPanel";
    panel.innerHTML = `
        <div class="tripx-chat-header">
            <div class="tripx-chat-avatar">TX</div>
            <div class="tripx-chat-heading">
                <strong>TripX Assistant</strong>
                <small><span class="tripx-online-dot"></span> Online · ${role === "admin" ? "Admin Support" : "Passenger Support"}</small>
            </div>
            <button id="tripxChatClear" type="button" title="Clear chat">↻</button>
            <button id="tripxChatClose" type="button" title="Close">×</button>
        </div>
        <div id="tripxChatMessages" class="tripx-chat-messages"></div>
        <div class="tripx-chat-suggestions">
            <button type="button" data-question="Mala seat book karaychi aahe">Book a seat</button>
            <button type="button" data-question="Available bus routes kontya aahet?">Bus routes</button>
            <button type="button" data-question="Fare kasa calculate hoto?">Fare help</button>
        </div>
        <form id="tripxChatForm" class="tripx-chat-input">
            <input id="tripxChatText" type="text" placeholder="Type in Marathi, Hinglish or English..." autocomplete="off">
            <button type="submit" title="Send">➤</button>
        </form>
        <div class="tripx-chat-footer">TripX · Your travel companion</div>
    `;

    document.body.appendChild(launcher);
    document.body.appendChild(panel);

    const messages = document.getElementById("tripxChatMessages");
    const input = document.getElementById("tripxChatText");

    function addMessage(text, sender, extraClass) {
        const item = document.createElement("div");
        item.className = "tripx-chat-message " + sender + (extraClass ? " " + extraClass : "");
        item.textContent = text;
        messages.appendChild(item);
        messages.scrollTop = messages.scrollHeight;
    }

    function typingReply(text) {
        const typing = document.createElement("div");
        typing.className = "tripx-chat-message bot tripx-typing";
        typing.textContent = "TripX is typing...";
        messages.appendChild(typing);
        messages.scrollTop = messages.scrollHeight;

        setTimeout(function () {
            typing.remove();
            addMessage(text, "bot");
        }, 450);
    }

    function detectLanguage(q) {
        if (/[अ-हऀ-ॿ]/.test(q)) return "marathi";
        if (/\b(mala|majha|majhi|kay|kasa|kashi|kuthe|aahe|ahe|karaycha|sanga|sang|nahi|ho|pahije|kiti|kontya|kashe)\b/i.test(q)) return "hinglish";
        return "english";
    }

    function replyFor(question) {
        const q = question.toLowerCase().trim();
        const lang = detectLanguage(q);
        const mr = lang === "marathi";
        const hi = lang === "hinglish";

        const say = function (en, hinglish, marathi) {
            return mr ? marathi : hi ? hinglish : en;
        };

        if (/^(hi|hello|hey|hii|namaskar|namaste)\b/.test(q)) {
            return say(
                "Hey " + name + "! Welcome back. What can I help you with today?",
                "Are " + name + "! Kay mhantos? TripX madhye kasli help havi?",
                "नमस्कार " + name + "! TripX मध्ये स्वागत आहे. आज कशी मदत करू?"
            );
        }

        if (/\b(thank|thanks|thx|dhany|abhaar)\b/.test(q)) {
            return say(
                "You're welcome! I'm here whenever you need help with your journey.",
                "Are welcome! Ajun kahi help havi asel tar bindhast vichar.",
                "तुमचं स्वागत आहे! प्रवासाबद्दल आणखी काही मदत हवी असेल तर नक्की विचारा."
            );
        }

        if (/\b(who are you|your name|kon ahes|tu kon|about you)\b/.test(q)) {
            return say(
                "I'm TripX Assistant, your travel helper. I can guide you around bookings, buses, fares and account features.",
                "Mi TripX Assistant aahe! Bus, booking, fare ani website use karayla madat karto.",
                "मी TripX Assistant आहे. Bus, booking, fare आणि website वापरण्याबद्दल मार्गदर्शन करू शकतो."
            );
        }

        if (/\b(book|booking|seat|ticket|reserve|बुक|तिकीट)\b/.test(q)) {
            lastTopic = "booking";
            return say(
                "To book a seat, open Seat Booking from the dashboard, choose your journey and bus, enter passenger details, then complete the booking. You can check your booking details there too.",
                "Seat book karaychi asel tar Dashboard varun Seat Booking open kar. Journey ani bus select kar, details bhar ani booking complete kar. Kuthlya step la problem yetoy?",
                "Seat book करण्यासाठी Dashboard मधून Seat Booking उघडा. Journey आणि bus निवडा, passenger details भरा आणि booking पूर्ण करा. कोणत्या step ला अडचण येतेय?"
            );
        }

        if (/\b(route|routes|bus|buses|timing|schedule|available|मार्ग|बस)\b/.test(q)) {
            lastTopic = "routes";
            return say(
                "Open Bus Management to view the available buses and routes. Sample routes include Pune–Bhusawal, Pune–Jalgaon, Pune–Dhule, Pune–Mumbai, Pune–Nashik and others.",
                "Bus routes baghayche astil tar Bus Management open kar. Aplyakade Pune–Bhusawal, Jalgaon, Dhule, Mumbai, Nashik ani itar routes aahet.",
                "Available buses आणि routes पाहण्यासाठी Bus Management उघडा. Pune–Bhusawal, Pune–Jalgaon, Pune–Dhule, Pune–Mumbai, Pune–Nashik असे sample routes आहेत."
            );
        }

        if (/\b(fare|price|cost|rate|paise|rupaye|किराया|भाडे|पैसे)\b/.test(q)) {
            lastTopic = "fare";
            return say(
                "You can use the Fare Calculator page. Enter your journey details to calculate the fare. The displayed fare depends on the route information configured in the project.",
                "Fare check karaycha asel tar Fare Calculator page open kar. Journey details taklyavar fare calculate karta yeil.",
                "Fare पाहण्यासाठी Fare Calculator page उघडा. Journey details भरल्यानंतर fare calculate करता येईल."
            );
        }

        if (/\b(cancel|cancellation|refund|परतावा|रद्द)\b/.test(q)) {
            lastTopic = "cancel";
            return say(
                "Open Seat Booking and check your booking details for cancellation options. Any refund depends on the cancellation rules configured in the project.",
                "Booking cancel karaychi asel tar Seat Booking madhye booking details check kar. Refund project madhil cancellation rules nusar asel.",
                "Booking cancel करण्यासाठी Seat Booking मधील booking details तपासा. Refund project मध्ये असलेल्या cancellation rules नुसार असेल."
            );
        }

        if (/\b(passenger|register|registration|customer|प्रवासी)\b/.test(q)) {
            lastTopic = "passenger";
            if (role === "admin") {
                return say(
                    "As Admin, you can open the Passengers page to register and manage passenger records.",
                    "Tu Admin ahes, mhanun Passengers page varun passenger register ani manage karu shakto.",
                    "तुम्ही Admin असल्यामुळे Passengers page वरून passenger records register आणि manage करू शकता."
                );
            }
            return say(
                "Your passenger account is linked to your login. For changes to your account details, please contact the Admin.",
                "Tujha passenger account login sobat linked aahe. Account details change karayche astil tar Admin la contact kar.",
                "तुमचं passenger account login शी जोडलेलं आहे. Account details बदलण्यासाठी Admin शी संपर्क साधा."
            );
        }

        if (/\b(add bus|new bus|bus add|bus create|नवीन बस)\b/.test(q)) {
            lastTopic = "busadmin";
            return role === "admin"
                ? say(
                    "As Admin, open Bus Management and use Add New Bus. Fill in the bus details and save.",
                    "Admin mhanun Bus Management open kar ani Add New Bus var click kar. Details bharun save kar.",
                    "Admin म्हणून Bus Management उघडा आणि Add New Bus वापरून bus details भरून save करा."
                )
                : say(
                    "Adding a bus is an Admin-only feature. You can still view available buses and routes.",
                    "New bus add karaycha option fakt Admin sathi aahe. Tu available buses ani routes pahu shakto.",
                    "नवीन bus add करण्याचा अधिकार फक्त Admin कडे आहे. तुम्ही available buses आणि routes पाहू शकता."
                );
        }

        if (/\b(what can you do|how can you help|tu kay karu shakto|kay help karshil|features)\b/.test(q)) {
            lastTopic = "general";
            return say(
                "I can chat with you about travelling, destinations, trip planning, budgets, packing, travel safety and how to use TripX. Just talk naturally! 😊",
                "Are, mi travelling, destinations, trip planning, budget, packing, safety ani TripX website baddal help karu shakto. Bindhast vichar! 😄",
                "मी travelling, destinations, trip planning, budget, packing, safety आणि TripX website बद्दल मदत करू शकतो. मोकळेपणाने विचारा! 😊"
            );
        }

        if (/\b(trip plan|plan my trip|itinerary|tour plan|trip schedule|pravasa cha plan|trip planning)\b/.test(q)) {
            lastTopic = "trip";
            return say(
                "Let's plan something exciting! 🌍 Tell me your starting city, destination, number of days and approximate budget. I'll help you organise a practical itinerary.",
                "Chal ek mast trip plan karuya! 🌍 Kutun nighnar, kuthe jaycha, kiti divas ani approx budget kiti aahe te sang.",
                "चला एक छान trip plan करूया! 🌍 कुठून निघणार, कुठे जायचं, किती दिवस आणि अंदाजे budget किती आहे ते सांगा."
            );
        }

        // More natural travel and general conversation support

        if (/\b(2-day|2 day|two day|day trip|trip from|days trip|suggest me a .* trip|plan .* trip)\b/.test(q)) {
            lastTopic = "trip";
            return say(
                "Absolutely! 🌿 For a 2-day nature trip from Pune, consider Lonavala and Khandala. Day 1: explore the viewpoints and nearby nature spots. Day 2: enjoy a relaxed visit around the hills and return to Pune. Other options include Mulshi and Tamhini. Check current transport, weather and local access before leaving.",
                "Pune varun 2 divasanchi nature trip mast hoil! 🌿 Day 1 Lonavala-Khandala explore kar, Day 2 nearby viewpoints ani nature enjoy karun Pune la parat ye. Mulshi ani Tamhini pan options aahet. Nighnyapurvi weather ani transport check kar.",
                "पुण्याहून २ दिवसांसाठी Lonavala-Khandala छान पर्याय आहे! 🌿 पहिल्या दिवशी viewpoints पाहा आणि दुसऱ्या दिवशी आसपासचा निसर्ग अनुभवून पुण्याला परत या. Mulshi आणि Tamhini हेही पर्याय आहेत. निघण्यापूर्वी हवामान आणि transport तपासा."
            );
        }

        if (/^(nature|mountains|beaches|forts|waterfalls|निसर्ग|डोंगर|समुद्र|किल्ले)$/i.test(q) && lastTopic === "destination") {
            lastTopic = "destination";
            return say(
                "Nature lover! 🌿 From Pune, you can explore Mulshi, Tamhini Ghat, Lonavala, Matheran and Mahabaleshwar. For a peaceful weekend, Mulshi or Tamhini could be lovely options. Want a 1-day or 2-day plan?",
                "Are wah, nature lover! 🌿 Pune varun Mulshi, Tamhini Ghat, Lonavala, Matheran ani Mahabaleshwar explore karu shakto. Peaceful trip sathi Mulshi kiwa Tamhini mast aahe. 1 day ki 2 days plan karu?",
                "वा! तुम्हाला निसर्ग आवडतो! 🌿 पुण्याहून Mulshi, Tamhini Ghat, Lonavala, Matheran आणि Mahabaleshwar येथे जाता येईल. शांत weekend साठी Mulshi किंवा Tamhini छान पर्याय आहेत. १ दिवसांचा की २ दिवसांचा plan हवा?"
            );
        }

        if (/\b(best hill stations|hill station|hill stations)\b/.test(q)) {
            lastTopic = "destination";
            return say(
                "Some popular hill stations in Maharashtra are Mahabaleshwar, Matheran, Lonavala, Panchgani and Amboli. 🏞️ From Pune, Lonavala and Mahabaleshwar are convenient options.",
                "Maharashtra madhye Mahabaleshwar, Matheran, Lonavala, Panchgani ani Amboli famous hill stations aahet. 🏞️ Pune varun Lonavala ani Mahabaleshwar convenient aahet.",
                "महाराष्ट्रातील प्रसिद्ध hill stations म्हणजे Mahabaleshwar, Matheran, Lonavala, Panchgani आणि Amboli. 🏞️ पुण्याहून Lonavala आणि Mahabaleshwar हे सोयीचे पर्याय आहेत."
            );
        }

        if (/\b(tourism and travelling|difference between tourism and travelling|tourism vs travelling)\b/.test(q)) {
            return say(
                "Travelling means going from one place to another for different reasons. Tourism is travelling mainly for leisure, recreation or exploring places. In simple words, tourism is one purpose of travelling. 🌍",
                "Travelling mhanje eka thikanahun dusrya thikani jane. Tourism mhanje mainly firnyasathi, enjoyment kiwa places explore karnyasathi travel karne. 🌍",
                "Travelling म्हणजे एका ठिकाणाहून दुसऱ्या ठिकाणी जाणे. Tourism म्हणजे मुख्यतः फिरण्यासाठी, आनंदासाठी किंवा ठिकाणं explore करण्यासाठी केलेला प्रवास. 🌍"
            );
        }

        if (/\b(something interesting about maharashtra|interesting fact about maharashtra|maharashtra facts)\b/.test(q)) {
            return say(
                "Here's a fun fact! Maharashtra has more than 350 forts associated with its rich history, including famous hill forts in the Sahyadri ranges. 🏰",
                "Maharashtra baddal ek interesting fact! Aplya rajyat 350 peksha jast forts aahet, ani Sahyadri madhil hill forts khup famous aahet. 🏰",
                "महाराष्ट्राबद्दल एक interesting गोष्ट! राज्यात ३५० पेक्षा जास्त किल्ले आहेत आणि सह्याद्रीतील डोंगरी किल्ले इतिहासासाठी प्रसिद्ध आहेत. 🏰"
            );
        }

        if (/\b(bus or train|which is better.*bus|better.*train|bus vs train)\b/.test(q)) {
            return say(
                "It depends on your journey! 🚌 Buses can offer flexible routes and convenient boarding points. Trains can be more comfortable for many longer journeys. Compare travel time, price and availability before deciding.",
                "Journey var depend aahe! 🚌 Bus madhye flexible routes milu shaktat, tar long journey sathi train comfortable asu shakते. Time, price ani availability compare kar.",
                "हे प्रवासावर अवलंबून आहे! 🚌 Bus मध्ये flexible routes मिळू शकतात, तर लांब प्रवासासाठी train सोयीची असू शकते. वेळ, किंमत आणि availability compare करा."
            );
        }

        if (/\b(confident traveller|confident traveler|become a confident traveller|become a confident traveler)\b/.test(q)) {
            return say(
                "Start with short trips, plan your route, keep emergency contacts, learn basic local phrases and trust your preparation. Every trip will build your confidence! 💪",
                "Suruvat chotya trips pasun kar. Route plan kar, emergency contacts thev ani basic preparation kar. Pratyek trip sobat confidence vadhat jail! 💪",
                "छोट्या trips पासून सुरुवात करा. Route plan करा, emergency contacts ठेवा आणि basic तयारी करा. प्रत्येक trip सोबत confidence वाढेल! 💪"
            );
        }

        if (/\b(real bus ticket|book a real bus|can you book.*ticket|actual ticket)\b/.test(q)) {
            return say(
                "I can guide you through the TripX booking process, but I cannot purchase or issue a real-world bus ticket from this offline chatbot. Please complete real bookings through an authorised booking service. 🎫",
                "Mi TripX booking process samjavun sangu shakto, pan ya offline chatbot madhun real bus ticket purchase kiwa issue karu shakat nahi. Real booking authorised service var kar. 🎫",
                "मी TripX booking process समजावून सांगू शकतो; पण या offline chatbot मधून खरे bus ticket purchase किंवा issue करू शकत नाही. प्रत्यक्ष booking authorised service वर करा. 🎫"
            );
        }

        if (/\b(without.*budget|don't ask.*budget|dont ask.*budget|no budget.*question)\b/.test(q)) {
            lastTopic = "destination";
            return say(
                "No problem! 😊 If you enjoy nature, consider Lonavala, Matheran or Mulshi from Pune. If you like history, try Raigad or Sinhagad. If you prefer beaches, explore the Konkan coast.",
                "Budget vicharat nahi! 😊 Nature sathi Lonavala, Matheran kiwa Mulshi; history sathi Raigad kiwa Sinhagad; beaches sathi Konkan explore kar.",
                "काही हरकत नाही! 😊 निसर्गासाठी Lonavala, Matheran किंवा Mulshi; इतिहासासाठी Raigad किंवा Sinhagad; beaches साठी Konkan coast explore करू शकता."
            );
        }

        if (/\b(only 500|₹500|rs 500|500 rupees|500 budget)\b/.test(q)) {
            return say(
                "Yes, a small local outing may be possible with ₹500! 💰 Choose a nearby destination, use affordable public transport, carry water and snacks, and check entry fees first. A full long-distance trip may need more money.",
                "₹500 madhye nearby outing possible hou shakते! 💰 Jawalcha place choose kar, public transport use kar, snacks gheun ja ani entry fees check kar. Long-distance trip sathi jast budget lagu shakto.",
                "₹५०० मध्ये जवळपासची छोटी outing शक्य होऊ शकते! 💰 जवळचं ठिकाण निवडा, public transport वापरा, snacks सोबत ठेवा आणि entry fees आधी तपासा. लांबच्या trip साठी जास्त budget लागू शकतो."
            );
        }

        if (/\b(what if i miss my bus|miss my bus|missed my bus|bus chukli|bus sutli)\b/.test(q)) {
            return say(
                "Don't panic. Contact the bus operator immediately, check whether rescheduling is possible, and keep your ticket and booking ID ready. Refunds or changes depend on the operator's policy. 🚌",
                "Panic nako karu. Bus operator la lagech contact kar, rescheduling possible aahe ka vichar ani ticket/booking ID ready thev. Refund operator policy var depend karto. 🚌",
                "घाबरू नका. लगेच bus operator शी संपर्क साधा, rescheduling शक्य आहे का विचारा आणि ticket व booking ID जवळ ठेवा. Refund operator च्या policy वर अवलंबून असतो. 🚌"
            );
        }

        if (/\b(guarantee.*hotel|hotel.*safe|hotel safety|is this hotel safe)\b/.test(q)) {
            return say(
                "I can't guarantee that a hotel is safe without verified, current information. Check recent reviews, location, official contact details and safety policies. Share your location with someone you trust. 🏨",
                "Verified current information shivay hotel safe aahe yachi guarantee deu shakat nahi. Recent reviews, location ani official details check kar. 🏨",
                "सध्याची verified माहिती नसताना hotel सुरक्षित असल्याची guarantee देता येणार नाही. Recent reviews, location आणि official details तपासा. 🏨"
            );
        }

        if (/\b(where did i say|where was i going|remember.*destination|what place did i mention|where did i want to go)\b/.test(q)) {
            const earlier = conversation.slice(0, -1).filter(function (item) {
                return item.role === "user";
            }).map(function (item) {
                return item.text;
            }).join(" ");

            const match = earlier.match(/\b(Pune|Mumbai|Nashik|Lonavala|Matheran|Mahabaleshwar|Mulshi|Tamhini|Bhusawal|Jalgaon|Dhule)\b/i);

            return match
                ? "You previously mentioned " + match[0] + ". 😊 I'm using our current chat context."
                : "I don't see a specific destination mentioned earlier in this chat yet. Tell me your starting city and destination, and I'll follow along. 😊";
        }

        if (/\b(are you my friend|tu majha friend|tu mazi friend|are we friends)\b/.test(q)) {
            return say(
                "Of course, I'm your friendly TripX travel companion! 😄 I can chat, help you plan trips and guide you around TripX.",
                "Ho re! 😄 Mi tuza friendly TripX companion aahe. Gappa, trip planning ani TripX help sathi mi ithe aahe.",
                "नक्कीच! 😄 मी तुमचा friendly TripX companion आहे. गप्पा, trip planning आणि TripX मदतीसाठी इथे आहे."
            );
        }

        if (/\b(motivate me|motivation|achieve my goals|encourage me)\b/.test(q)) {
            return say(
                "Remember, big goals are achieved through small consistent steps. Don't compare your journey with someone else's. Keep learning, keep showing up and trust yourself. You've got this! 💪✨",
                "Lakshat thev, mothi goals chotya consistent steps ne achieve hotat. Dusryanshi comparison nako. Daily thoda progress kar. Tu nakkich karu shakto! 💪✨",
                "लक्षात ठेवा, मोठी ध्येयं छोट्या सातत्यपूर्ण प्रयत्नांनी पूर्ण होतात. स्वतःची तुलना इतरांशी करू नका. रोज थोडी प्रगती करा. तुम्ही नक्की करू शकता! 💪✨"
            );
        }

        if (/\b(mood is good|feeling happy|happy today|mood khup chhan|mood changla)\b/.test(q)) {
            return say(
                "That's lovely! 😄 Keep that positive energy going. Any exciting plans today?",
                "Are wah! 😄 Asaach happy raha. Aaj kahi special plan aahe ka?",
                "वा! 😄 असाच आनंदी राहा. आज काही खास plan आहे का?"
            );
        }

        if (/\b(do you like travelling|do you enjoy travelling|you like travel)\b/.test(q)) {
            return say(
                "I don't travel personally, but I love helping people discover places and plan exciting journeys! 🌍",
                "Mi personally travel karat nahi, pan lokanna navin places discover karayla ani trips plan karayla help karayla mala avadta! 🌍",
                "मी स्वतः प्रवास करत नाही, पण लोकांना नवीन ठिकाणं शोधायला आणि छान trips plan करायला मदत करायला मला आवडतं! 🌍"
            );
        }

        if (/\b(different from google|different than google|what makes you different)\b/.test(q)) {
            return say(
                "Google helps you search across the web. I'm a conversational assistant inside TripX. I can understand your questions in context and guide you through this project, but I don't have live web search in this offline version. 😊",
                "Google web search karayla help karto; mi TripX madhla conversational assistant aahe. Mi context nusar guide karu shakto, pan ya offline version madhye live web search nahi. 😊",
                "Google वेबवर माहिती शोधायला मदत करतं; मी TripX मधील conversational assistant आहे. मी संदर्भानुसार मार्गदर्शन करू शकतो, पण या offline version मध्ये live web search नाही. 😊"
            );
        }

        if (/\b(what is the current weather|current weather|weather right now|weather today)\b/.test(q)) {
            return "I don't have live weather access in this offline version. 🌦️ Please check a current weather app for the latest forecast before travelling.";
        }
        if (/\b(destination|places to visit|where should i go|best place|tourist place|kuthe firayla|firayla jaycha|travel place)\b/.test(q)) {
            lastTopic = "destination";
            return say(
                "There are so many beautiful places to explore! 🏞️ Tell me your starting city and whether you prefer mountains, beaches, forts, nature or city experiences. I'll suggest suitable destinations.",
                "Mast places khup aahet! 🏞️ Tu kutun nighnar ani tula mountains, beaches, forts ki nature avadta te sang. Mi destinations suggest karto.",
                "फिरण्यासाठी खूप सुंदर ठिकाणं आहेत! 🏞️ तुम्ही कुठून निघणार आणि mountains, beaches, forts की nature यापैकी काय आवडतं ते सांगा."
            );
        }

        if (/\b(budget|cheap trip|low cost|save money|affordable|swast|kami kharcha|paise vachav)\b/.test(q)) {
            lastTopic = "budget";
            return say(
                "Let's make your trip budget-friendly! 💰 Compare transport options, book early, travel in off-peak periods and keep some money aside for food and emergencies. Tell me your budget and destination for a better plan.",
                "Budget madhye trip karaychi asel tar early booking kar, transport compare kar ani food/emergency sathi paise vegle thev. 💰 Budget ani destination sang, mi plan karayla help karto.",
                "कमी budget मध्ये trip करायची असेल तर आधी booking करा, transport options compare करा आणि food व emergency साठी पैसे वेगळे ठेवा. 💰 तुमचं budget आणि destination सांगा."
            );
        }

        if (/\b(pack|packing|luggage|bag|what to carry|kay gheu|saman|kapde)\b/.test(q)) {
            lastTopic = "packing";
            return say(
                "Packing checklist 🎒: ID proof, phone and charger, medicines, suitable clothes, toiletries, water bottle and booking details. For hills or rain, carry weather-appropriate layers. Where are you travelling?",
                "Packing kartana ID, phone-charger, medicines, kapde, toiletries, water bottle ani booking details ghe. 🎒 Hills kiwa rainy place asel tar tyapramane kapde ghe. Kuthe challay?",
                "Packing checklist 🎒: ID proof, phone आणि charger, औषधं, योग्य कपडे, toiletries, पाण्याची बाटली आणि booking details सोबत ठेवा. कुठे प्रवास करणार आहात?"
            );
        }

        if (/\b(safe|safety|solo travel|alone travel|travel alone|surakshit|kalji)\b/.test(q)) {
            lastTopic = "safety";
            return say(
                "A few travel safety habits: share your itinerary with someone you trust, keep emergency contacts handy, use verified transport and accommodation, protect your belongings and avoid sharing sensitive details with strangers. ❤️",
                "Travel safety sathi itinerary gharachyana share kar, emergency contacts thev, verified transport use kar ani belongings sambhal. ❤️ Solo travel asel tar extra planning kar.",
                "प्रवासात सुरक्षिततेसाठी itinerary घरच्यांना share करा, emergency contacts जवळ ठेवा, verified transport वापरा आणि सामान सांभाळा. ❤️"
            );
        }

        if (/\b(weather|rain|temperature|climate|mausam|pavs|pाऊस)\b/.test(q)) {
            lastTopic = "weather";
            return say(
                "Weather can change your travel plans! 🌦️ I don't have live weather updates here, so check a current weather forecast for your destination before leaving. Which place are you visiting?",
                "Weather mule plan change hou shakto! 🌦️ Majhyakade live weather updates nahit. Nighnyapurvi current forecast check kar. Kontya place la challay?",
                "हवामानामुळे प्रवासाचा plan बदलू शकतो! 🌦️ माझ्याकडे live weather updates नाहीत. निघण्यापूर्वी destination चा current forecast तपासा. कुठे जाणार आहात?"
            );
        }

        if (/\b(train|railway|flight|airplane|flight ticket|bus vs train|transport options)\b/.test(q)) {
            lastTopic = "transport";
            return say(
                "Choosing transport depends on distance, budget, travel time and comfort. 🚌 Trains can be economical for many routes, flights save time on long distances, and buses offer flexible road connections. Tell me your from-to cities.",
                "Transport choose kartana distance, budget ani time bagh. 🚌 Train budget-friendly asu shakते, flight time vachavte ani bus road connectivity dete. From-to cities sang.",
                "Transport निवडताना अंतर, budget, वेळ आणि comfort पाहा. 🚌 Train, flight आणि bus यांचे पर्याय तुलना करता येतील. तुमची starting आणि destination city सांगा."
            );
        }

        if (/\b(hotel|stay|accommodation|where to stay|room booking|rahaycha|hotel booking)\b/.test(q)) {
            lastTopic = "stay";
            return say(
                "For a good stay, compare location, recent reviews, safety, cancellation policy and total price before booking. 🏨 Which destination and budget per night are you considering?",
                "Stay book kartana location, recent reviews, safety ani total price check kar. 🏨 Kontya destination la jaycha ani per-night budget kiti aahe?",
                "Stay book करताना location, recent reviews, safety, cancellation policy आणि total price तपासा. 🏨 कोणत्या ठिकाणी आणि किती budget मध्ये राहायचं आहे?"
            );
        }

        if (/\b(food|eat|famous food|local food|cuisine|khaayla|jevan|food places)\b/.test(q)) {
            lastTopic = "food";
            return say(
                "Trying local food is one of the best parts of travelling! 🍜 Tell me your destination and food preference. I can suggest local dishes and what to look for.",
                "Local food try karna trip cha best part asto! 🍜 Destination ani veg/non-veg preference sang, mi famous dishes suggest karto.",
                "स्थानिक पदार्थ चाखणं हा प्रवासाचा छान भाग असतो! 🍜 Destination आणि food preference सांगा, मी प्रसिद्ध dishes सुचवतो."
            );
        }

        if (/\b(tripx|what is tripx|about tripx|this website|our website|website features)\b/.test(q)) {
            lastTopic = "tripx";
            return say(
                "TripX is our bus travel management project. 🚌 It brings together bus routes, seat booking, fare calculation, passenger information and bus management in one website. I'm here to help you use it!",
                "TripX aapla bus travel management project aahe! 🚌 Yat bus routes, seat booking, fare calculation, passenger information ani bus management features aahet.",
                "TripX हा आपला bus travel management project आहे. 🚌 यात bus routes, seat booking, fare calculation, passenger information आणि bus management features आहेत."
            );
        }

        if (/\b(joke|make me laugh|funny|hasav| विनोद)\b/.test(q)) {
            return say(
                "Why did the bus get promoted? Because it always went the extra mile! 😂🚌",
                "Bus driver la promotion ka milala? Karan to nehmi extra mile jaycha! 😂🚌",
                "बसला promotion का मिळालं? कारण ती नेहमी extra mile जायची! 😂🚌"
            );
        }

        if (/\b(sad|upset|bored|tired|stress|lonely|mood off|dukhi| कंटाळा)\b/.test(q)) {
            return say(
                "Hey, take it easy. ❤️ Sometimes a little break, music or talking to someone helps. Want to tell me what's going on?",
                "Are, relax kar na. ❤️ Thoda break ghe, music aik kiwa bolaycha asel tar mi aahe. Kay zala?",
                "अरे, थोडं शांत हो. ❤️ छोटासा break घे, आवडतं music ऐक किंवा बोलायचं असेल तर सांग. काय झालं?"
            );
        }

        if (/\b(good morning|good afternoon|good evening|good night| शुभ रात्री)\b/.test(q)) {
            return say(
                "Hope your day is going great! ☀️ What are we exploring today?",
                "Good vibes! ☀️ Aaj kay explore karaycha?",
                "तुमचा दिवस छान जावो! ☀️ आज काय explore करायचं?"
            );
        }
        if (/\b(admin|dashboard|website|system|help|problem|issue|error)\b/.test(q)) {
            lastTopic = "general";
            return role === "admin"
                ? say(
                    "I can help you with bus management, passenger records, bookings and fare information. Tell me what you're trying to do, and I'll guide you step by step.",
                    "Admin panel madhye bus management, passenger records, bookings ani fare sathi mi guide karu shakto. Nemka kay karaycha aahe te sang.",
                    "Bus management, passenger records, bookings आणि fare यासाठी मी मदत करू शकतो. तुम्हाला नेमकं काय करायचं आहे ते सांगा."
                )
                : say(
                    "I can guide you with bus routes, seat booking, fare calculation and cancellation. Tell me what you're trying to do.",
                    "Mi bus routes, seat booking, fare ani cancellation madhye help karu shakto. Tula nemka kay karaycha aahe?",
                    "मी bus routes, seat booking, fare आणि cancellation याबद्दल मदत करू शकतो. तुम्हाला नेमकं काय करायचं आहे?"
                );
        }

        if (/\b(more|explain|details|tell me more|continue|pudhe|ajun sang|tyabaddal|elaborate|काय पुढे)\b/.test(q)) {
            const followups = {
                booking: say(
                    "Let's do it step by step: open Seat Booking, select the journey, choose a bus and fill in the passenger details. Which step are you currently on?",
                    "Chal step by step jau: Seat Booking open kar, journey select kar, bus choose kar ani passenger details bhar. Tu atta kontya step var ahes?",
                    "चला step by step करूया: Seat Booking उघडा, journey निवडा, bus निवडा आणि passenger details भरा. तुम्ही सध्या कोणत्या step वर आहात?"
                ),
                routes: say(
                    "You can open Bus Management to see the route list. Which route are you looking for?",
                    "Bus Management madhye route list disel. Tula konta route pahije?",
                    "Bus Management मध्ये route list दिसेल. तुम्हाला कोणता route पाहिजे?"
                ),
                fare: say(
                    "Open Fare Calculator and enter the journey details. Are you trying to calculate a particular route?",
                    "Fare Calculator open karun journey details tak. Kontya route cha fare pahije?",
                    "Fare Calculator उघडून journey details भरा. कोणत्या route चं fare पाहिजे?"
                ),
                cancel: say(
                    "Check the booking details on the Seat Booking page first. Are you trying to cancel a booking or understand the refund?",
                    "Seat Booking madhye booking details check kar. Tula booking cancel karaychi aahe ki refund samjun ghaycha aahe?",
                    "आधी Seat Booking मधील booking details तपासा. तुम्हाला booking cancel करायची आहे की refund समजून घ्यायचा आहे?"
                )
            };
            if (followups[lastTopic]) return followups[lastTopic];
        }

        if (/\b(ok|okay|cool|great|barobar|thik|thik aahe|chhan)\b/.test(q)) {
            return say(
                "Great! Let me know if you need anything else.",
                "Mast! Ajun kahi lagla tar vichar.",
                "छान! आणखी काही मदत हवी असेल तर विचारा."
            );
        }

        return say(
            "I may not have understood that fully. Try asking me about bus routes, seat booking, fare, cancellation or account help. You can also tell me what you're trying to do.",
            "He mala purnapane samajla nahi 😅 Bus routes, booking, fare, cancellation baddal vichar. Kinva tula nemka kay karaycha aahe te sang, mi guide karto.",
            "मला तुमचा प्रश्न पूर्णपणे समजला नाही 😅 Bus routes, booking, fare किंवा cancellation बद्दल विचारा. किंवा तुम्हाला नेमकं काय करायचं आहे ते सांगा."
        );
    }

    function ask(question) {
        const clean = question.trim();
        if (!clean) return;

        addMessage(clean, "user");
        conversation.push({ role: "user", text: clean });
        const answer = replyFor(clean);
        conversation.push({ role: "assistant", text: answer });
        if (conversation.length > 12) conversation = conversation.slice(-12);
        typingReply(answer);
    }

    function welcome() {
        const greeting = "Hi " + name + "! I'm your TripX Assistant. Bol, kasa help karu? You can chat in Marathi, Hinglish or English.";
        addMessage(greeting, "bot");
    }

    launcher.addEventListener("click", function () {
        panel.classList.toggle("open");
        if (panel.classList.contains("open")) input.focus();
    });

    document.getElementById("tripxChatClose").addEventListener("click", function () {
        panel.classList.remove("open");
    });

    document.getElementById("tripxChatClear").addEventListener("click", function () {
        messages.innerHTML = "";
        conversation = [];
        lastTopic = "general";
        welcome();
    });

    document.getElementById("tripxChatForm").addEventListener("submit", function (event) {
        event.preventDefault();
        ask(input.value);
        input.value = "";
        input.focus();
    });

    panel.querySelectorAll("[data-question]").forEach(function (button) {
        button.addEventListener("click", function () {
            ask(button.getAttribute("data-question"));
        });
    });

    welcome();
})();


