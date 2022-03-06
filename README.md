# enigma

```javascript --run
console.log('Hello, World!');
```

{% for image in rapport/jpg/ %}
  ![image]({{ rapport/jpg/ }} 'image')
{% endfor %}

{% for image in rapport/jpg/ %}
  ![image]({{ ./rapport/jpg/ }} 'image')
{% endfor %}

![image](./rapport/jpg/rapport-1.jpg)
![image](./rapport/jpg/rapport-2.jpg)
![image](./rapport/jpg/rapport-3.jpg)
![image](./rapport/jpg/rapport-4.jpg)


