import sys
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def send_email(subject, body):
    sender_email = "exmaple@gmail.com"  # 送信者のメールアドレス
    receiver_email = "example2@gmail.com"  # 受信者のメールアドレス
    password = "password"  # 送信者のメールパスワード

    try:
        # メールの構築
        message = MIMEMultipart()
        message["From"] = sender_email
        message["To"] = receiver_email
        message["Subject"] = subject

        # メール本文の追加
        message.attach(MIMEText(body, "plain"))

        # SMTPサーバーに接続してメールを送信
        with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
            server.login(sender_email, password)
            server.send_message(message)
        print("Email sent successfully")

    except smtplib.SMTPAuthenticationError:
        print("Authentication error: Could not log in to the SMTP server. Please check your username and password.")
    except smtplib.SMTPConnectError:
        print("Connection error: Could not connect to the SMTP server. Please check the server address and port.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    # コマンドライン引数から件名と内容を取得
    if len(sys.argv) < 3:
        print("Usage: python SendMail.py <subject> <body>")
        sys.exit(1)

    subject = sys.argv[1]
    body = sys.argv[2]

    # メールの送信
    send_email(subject, body)
