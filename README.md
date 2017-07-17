# ProjectorSimulator

템플릿 코드

- 테스트
Mac 64bit debug 모드에서 실행
실행하면 3차원 그리드내에 3개의 projector 오브젝트와 free camera로 둘러보기가 가능

- Projector.h, cpp 설명
Projector 클래스는 프로젝터 기계를 의미
XYZ좌표이동과 XYZ축회전 가능
실제 프로젝터의 스펙을 멤버변수로 이용할 예정
각 프로젝터마다 개별적으로 사용할 영상을 지정할 예정

- 현 구현상황
기본 제공된 ofEasyCam으로 free camera 구현
projector 오브젝트는 마우스로 선택이 가능. 선택된 프로젝터의 숫자가 좌측상단에 표시
선택된 projector가 있는 경우 d버튼을 통해 삭제가 가능

- TODO
1. 도량 통일 : 좌표계에서의 1씩 이동을 실세계의 몇cm,m로 환산할지?
2. 3D 모델링을 File Dialog로 불러오기
3. 3D 모델링을 관리할 클래스 구현(Projector 클래스처럼, Scale, Translate, Rotate 기능필수)
4. Projector 객체별로 영상을 File Dialog로 불러오기
5. Projector 객체의 비디오 플레이백 기능 구현(GUI 패널에서 버튼 클릭?)
6. easyCam 이용을 보다 쉽게(마우스 이동을 포함해서 키보드로까지)
7. Projector 및 3D 모델링에 쓰일 GIZMO 구현
8. 모델링에 영상을 비출 방법 모색
