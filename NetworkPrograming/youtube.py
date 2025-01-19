import yt_dlp

def download_video(url, save_path='.'):
    try:
        ydl_opts = {
            'outtmpl': f'{save_path}/%(title)s.%(ext)s',  # Output file template
            'format': 'bestvideo[height<=?1080][fps<=?60]+bestaudio/best',  # 1080p, 60fps, best audio
            'merge_output_format': 'mp4',  # Merges video + audio into MP4 format
        }
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            print(f"Downloading video from: {url}")
            ydl.download([url])
            print("Download complete!")
    except Exception as e:
        print(f"An error occurred: {e}")

# Input the video URL
video_url = input("Enter the URL of the YouTube video: ")
download_video(video_url)
