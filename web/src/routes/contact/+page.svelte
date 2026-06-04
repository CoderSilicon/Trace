<script lang="ts">
  const supportEmail = 'staticcodersilicon@gmail.com';
  

  const mailtoLink = `mailto:${supportEmail}?subject=sget%20CLI%20Inquiry`;
</script>

<div 
  class="min-h-screen bg-[#060709] text-white pb-24 selection:bg-[#00f5c4]/30 flex flex-col items-center"
>
  <main class="w-full max-w-2xl px-6 py-16 md:py-24">

    <div class="mb-12 border-b border-white/5 pb-8">
      <h1 class="jetbrains-mono-800 text-3xl md:text-4xl tracking-tight text-white mb-4">
        Contact
      </h1>
      <p class="jetbrains-mono-400 text-[#8e929a] text-sm md:text-base leading-relaxed">
        Have a question, found a bug, or just want to chat about command-line tools? Here is how you can reach the maintainer.
      </p>
    </div>

    <div class="space-y-6">

      <div class="bg-[#0d0f13] border border-white/5 rounded-md p-8 transition-colors hover:border-white/15 shadow-xl relative overflow-hidden group">
        
        <div class="absolute inset-0 bg-linear-to-br from-[#00f5c4]/3 to-transparent opacity-0 group-hover:opacity-100 transition-opacity pointer-events-none"></div>

        <h2 class="jetbrains-mono-700 text-xl text-white mb-2 flex items-center gap-3">
          <svg class="w-5 h-5 text-[#00f5c4]" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M3 8l7.89 5.26a2 2 0 002.22 0L21 8M5 19h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v10a2 2 0 002 2z"/>
          </svg>
          Direct Email
        </h2>
        
        <p class="jetbrains-mono-400 text-[#8e929a] text-sm leading-relaxed mb-8">
          For security reports, business inquiries, or private feedback, drop an email. I typically respond within 24-48 hours.
        </p>
        
        <div class="flex flex-col sm:flex-row sm:items-center gap-4">
          <a 
            href={mailtoLink} 
            class="bg-[#00f5c4] text-[#060709] px-6 py-3 rounded font-sans font-bold text-sm inline-flex justify-center items-center gap-2 hover:opacity-90 transition-opacity"
          >
            Send Message
          </a>
          
          <div class="jetbrains-mono-400 text-xs text-[#545861] bg-[#060709] border border-white/5 rounded px-4 py-3 select-all">
            {supportEmail}
          </div>
        </div>
      </div>

      <div class="bg-[#0d0f13] border border-white/5 rounded-md p-8 transition-colors hover:border-white/10 shadow-lg">
        <h2 class="jetbrains-mono-700 text-lg text-white mb-2 flex items-center gap-3">
          <svg class="w-5 h-5 text-[#8e929a]" fill="currentColor" viewBox="0 0 24 24">
            <path fill-rule="evenodd" clip-rule="evenodd" d="M12 2C6.477 2 2 6.484 2 12.017c0 4.425 2.865 8.18 6.839 9.504.5.092.682-.217.682-.483 0-.237-.008-.868-.013-1.703-2.782.605-3.369-1.343-3.369-1.343-.454-1.158-1.11-1.466-1.11-1.466-.908-.62.069-.608.069-.608 1.003.07 1.531 1.032 1.531 1.032.892 1.53 2.341 1.088 2.91.832.092-.647.35-1.088.636-1.338-2.22-.253-4.555-1.113-4.555-4.951 0-1.093.39-1.988 1.029-2.688-.103-.253-.446-1.272.098-2.65 0 0 .84-.27 2.75 1.026A9.564 9.564 0 0112 6.844c.85.004 1.705.115 2.504.337 1.909-1.296 2.747-1.027 2.747-1.027.546 1.379.202 2.398.1 2.651.64.7 1.028 1.595 1.028 2.688 0 3.848-2.339 4.695-4.566 4.943.359.309.678.92.678 1.855 0 1.338-.012 2.419-.012 2.747 0 .268.18.58.688.482A10.019 10.019 0 0022 12.017C22 6.484 17.522 2 12 2z"/>
          </svg>
          Issue Tracker
        </h2>
        <p class="jetbrains-mono-400 text-[#8e929a] text-sm leading-relaxed mb-6">
          Encountered a bug or have a feature request? The best place for code-related discussions and transparency is directly on the repository.
        </p>
        <a 
          href="https://github.com/CoderSilicon/sget" 
          target="_blank" 
          rel="noopener noreferrer"
          class="jetbrains-mono-500 text-sm text-[#8e929a] hover:text-white transition-colors flex items-center gap-2 group"
        >
          Open an issue <span class="group-hover:translate-x-1 transition-transform">&rarr;</span>
        </a>
      </div>

    </div>
  </main>
</div>
